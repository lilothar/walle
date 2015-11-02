#include <walle/net/Timer.h>
#include <walle/sys/wallesys.h>
#include <walle/net/Eventloop.h>
#include <walle/net/Timer.h>
#include <walle/net/TimerId.h>
#include <stdint.h>
#include <boost/bind.hpp>

#include <sys/timerfd.h>
using namespace walle::sys; 
using namespace walle::net; 

namespace walle {

namespace util{

int createTimerfd()
{
  int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                 TFD_NONBLOCK | TFD_CLOEXEC);
  if (timerfd < 0)
  {
    LOG_ERROR<<"Failed in timerfd_create";
  }
  return timerfd;
}

struct timespec fromNow(Time when)
{
	Time diff = when - Time::now();

  int64_t microseconds = diff.toMicroSeconds();
  if (microseconds < 100)
  {
    microseconds = 100;
  }
  struct timespec ts;
  ts.tv_sec = static_cast<time_t>(
      microseconds / Time::kMicroSecondsPerSecond);
  ts.tv_nsec = static_cast<long>(
      (microseconds % Time::kMicroSecondsPerSecond) * 1000);
  return ts;
}

void readTimerfd(int timerfd, Time now)
{
  uint64_t howmany;
  ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
  if (n != sizeof howmany) {
    LOG_ERROR<< "TimerQueue::handleRead() reads "<<n<<"bytes instead of 8";
  }
}

void resetTimerfd(int timerfd, Time expiration)
{
  // wake up loop by timerfd_settime()
  struct itimerspec newValue;
  struct itimerspec oldValue;
  bzero(&newValue, sizeof newValue);
  bzero(&oldValue, sizeof oldValue);
  newValue.it_value = fromNow(expiration);
  int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
  if (ret)
  {
   LOG_ERROR<<"timerfd_settime()";
  }
}
}
namespace net {
Timer::Timer(EventLoop* loop)
  : _loop(loop),
    _timerfd(util::createTimerfd()),
    _timerfdChannel(loop, _timerfd),
    _tasks(),
    _callingExpiredTasks(false)
{
  _timerfdChannel.setReadCallback(
      boost::bind(&Timer::handleRead, this));
  // we are always reading the timerfd, we disarm it with timerfd_settime.
  _timerfdChannel.enableReading();
}

Timer::~Timer()
{
  _timerfdChannel.disableAll();
  _timerfdChannel.remove();
  ::close(_timerfd);
  // do not remove channel, since we're in EventLoop::dtor();
  for (TaskList::iterator it = _tasks.begin();
      it != _tasks.end(); ++it)
  {
    delete it->second;
  }
}

TimerId Timer::addTimer(const TimerCallback& cb,
                             Time when,
                             int64_t interval)
{
  TimerTask* task = new TimerTask(cb, when, interval);
  _loop->runInLoop( boost::bind(&Timer::addTimerInLoop, this, task));
  return TimerId(task, task->sequence());
}

void Timer::cancel(TimerId timerId)
{
  _loop->runInLoop(
      boost::bind(&Timer::cancelInLoop, this, timerId));
}

void Timer::addTimerInLoop(TimerTask* task)
{
  _loop->assertInLoopThread();
  bool earliestChanged = insert(task);

  if (earliestChanged)
  {
    util::resetTimerfd(_timerfd, task->expiration());
  }
}

void Timer::cancelInLoop(TimerId timerId)
{
  _loop->assertInLoopThread();
  assert(_tasks.size() == _activeTasks.size());
  ActiveTask task(timerId._task, timerId._sequence);
  ActiveTaskSet::iterator it = _activeTasks.find(task);
  if (it != _activeTasks.end())
  {
    size_t n = _tasks.erase(Entry(it->first->expiration(), it->first));
    assert(n == 1); (void)n;
    delete it->first; // FIXME: no delete please
    _activeTasks.erase(it);
  }
  else if (_callingExpiredTasks)
  {
    _cancelingTasks.insert(task);
  }
  assert(_tasks.size() == _activeTasks.size());
}

void Timer::handleRead()
{
  _loop->assertInLoopThread();
  Time now(Time::now());
  util::readTimerfd(_timerfd, now);

  std::vector<Entry> expired = getExpired(now);

  _callingExpiredTasks = true;
  _cancelingTasks.clear();
  // safe to callback outside critical section
  for (std::vector<Entry>::iterator it = expired.begin();
      it != expired.end(); ++it)
  {
    it->second->run();
  }
  _callingExpiredTasks = false;

  reset(expired, now);
}

std::vector<Timer::Entry> Timer::getExpired(Time now)
{
  assert(_tasks.size() == _activeTasks.size());
  std::vector<Entry> expired;
  Entry sentry(now, reinterpret_cast<TimerTask*>(0xffffffffffffffff));
  TaskList::iterator end = _tasks.lower_bound(sentry);
  assert(end == _tasks.end() || now < end->first);
  std::copy(_tasks.begin(), end, back_inserter(expired));
  _tasks.erase(_tasks.begin(), end);

  for (std::vector<Entry>::iterator it = expired.begin();
      it != expired.end(); ++it)
  {
    ActiveTask task(it->second, it->second->sequence());
    size_t n = _activeTasks.erase(task);
    assert(n == 1); (void)n;
  }

  assert(_tasks.size() == _activeTasks.size());
  return expired;
}

void Timer::reset(const std::vector<Entry>& expired, Time now)
{
  Time nextExpire;

  for (std::vector<Entry>::const_iterator it = expired.begin();
      it != expired.end(); ++it)
  {
    ActiveTask timer(it->second, it->second->sequence());
    if (it->second->repeat()
        && _cancelingTasks.find(timer) == _cancelingTasks.end())
    {
      it->second->restart(now);
      insert(it->second);
    }
    else
    {
      // FIXME move to a free list
      delete it->second; // FIXME: no delete please
    }
  }

  if (!_tasks.empty())
  {
    nextExpire = _tasks.begin()->second->expiration();
  }

  if (nextExpire.valid())
  {
    util::resetTimerfd(_timerfd, nextExpire);
  }
}

bool Timer::insert(TimerTask* task)
{
  _loop->assertInLoopThread();
  assert(_tasks.size() == _activeTasks.size());
  bool earliestChanged = false;
  Time when = task->expiration();
  TaskList::iterator it = _tasks.begin();
  if (it == _tasks.end() || when < it->first)
  {
    earliestChanged = true;
  }
  {
    std::pair<TaskList::iterator, bool> result
      = _tasks.insert(Entry(when, task));
    assert(result.second); (void)result;
  }
  {
    std::pair<ActiveTaskSet::iterator, bool> result
      = _activeTasks.insert(ActiveTask(task, task->sequence()));
    assert(result.second); (void)result;
  }

  assert(_tasks.size() == _activeTasks.size());
  return earliestChanged;
}

}
}
