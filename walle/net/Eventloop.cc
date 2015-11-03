#include <walle/sys/Logging.h>
#include <walle/sys/Mutex.h>
#include <walle/net/Channel.h>
#include <walle/net/Poller.h>
#include <walle/net/Socket.h>
#include <walle/net/Timer.h>
#include <walle/net/Eventloop.h>
#include <pthread.h>
#include <signal.h>
#include <sys/eventfd.h>

using namespace walle::sys;
using namespace walle::net;

namespace {
__thread EventLoop* t_loopInThisThread = 0;

const int kPollTimeMs = 10000;

int createEventfd()
{
  int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (evtfd < 0)
  {
    LOG_ERROR<<"Failed in eventfd";
    abort();
  }
  return evtfd;
}

#pragma GCC diagnostic ignored "-Wold-style-cast"
class IgnoreSigPipe
{
 public:
  IgnoreSigPipe()
  {
    ::signal(SIGPIPE, SIG_IGN);
  }
};
#pragma GCC diagnostic error "-Wold-style-cast"

IgnoreSigPipe initObj;
}

EventLoop* EventLoop::getEventLoopOfCurrentThread()
{
  return t_loopInThisThread;
}

EventLoop::EventLoop()
  : _looping(false),
    _quit(false),
    _eventHandling(false),
    _callingPendingFunctors(false),
    _iteration(0),
    _threadId(walle::LocalThread::tid()),
    _poller(new Poller(this)),
    _timer(new Timer(this)),
    _wakeupFd(createEventfd()),
    _wakeupChannel(new Channel(this, _wakeupFd)),
    _currentActiveChannel(NULL)
{

  if (t_loopInThisThread)
  {
    LOG_ERROR<<"Another EventLoop "<< _threadId<<"exists in this thread";
  }
  else
  {
    t_loopInThisThread = this;
  }
  _wakeupChannel->setReadCallback(
      std::bind(&EventLoop::handleRead, this));
  // we are always reading the wakeupfd
  _wakeupChannel->enableReading();
  LOG_DEBUG<<"EventLoop created "<<this<<" in thread "<<_threadId;
}

EventLoop::~EventLoop()
{
  _wakeupChannel->disableAll();
  _wakeupChannel->remove();
  ::close(_wakeupFd);
  t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
  assert(!_looping);
  assertInLoopThread();
  _looping = true;
  _quit = false; 

  while (!_quit)
  {
    _activeChannels.clear();
    _pollReturnTime = _poller->poll(kPollTimeMs, &_activeChannels);
    ++_iteration;
	LOG_DEBUG<<"iteration:"<<_iteration;
    // TODO sort channel by priority
    _eventHandling = true;
    for (ChannelList::iterator it = _activeChannels.begin();
        it != _activeChannels.end(); ++it) {
     	 _currentActiveChannel = *it;
    	 _currentActiveChannel->handleEvent(_pollReturnTime);
    }
    _currentActiveChannel = NULL;
    _eventHandling = false;
    doPendingFunctors();
  }
  _looping = false;
}

void EventLoop::quit()
{
  _quit = true;
  // There is a chance that loop() just executes while(!quit_) and exists,
  // then EventLoop destructs, then we are accessing an invalid object.
  // Can be fixed using mutex_ in both places.
  if (!isInLoopThread())
  {
    wakeup();
  }
}

void EventLoop::runInLoop(const Functor& cb)
{
  if (isInLoopThread())
  {
    cb();
  }
  else
  {
    queueInLoop(cb);
  }
}

void EventLoop::queueInLoop(const Functor& cb)
{
  {

   ScopeMutex lock(&_mutex);
   _pendingFunctors.push_back(cb);
  }

  if (!isInLoopThread() || _callingPendingFunctors)
  {
    wakeup();
  }
}

TimerId EventLoop::runAt(const Time& time, const TimerCallback& cb)
{
  return _timer->addTimer(cb, time, 0);
}

TimerId EventLoop::runAfter(int64_t delay, const TimerCallback& cb)
{
  Time time(Time::now() + delay);
  return runAt(time, cb);
}

TimerId EventLoop::runEvery(int64_t interval, const TimerCallback& cb)
{
  Time time(Time::now() + interval);
  return _timer->addTimer(cb, time, interval);
}

void EventLoop::cancel(TimerId timerId)
{
  return _timer->cancel(timerId);
}

void EventLoop::updateChannel(Channel* channel)
{
  assert(channel->ownerLoop() == this);
  assertInLoopThread();
  _poller->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
  assert(channel->ownerLoop() == this);
  assertInLoopThread();
  if (_eventHandling)
  {
    assert(_currentActiveChannel == channel ||
        std::find(_activeChannels.begin(), _activeChannels.end(), channel) == 
					_activeChannels.end());
  }
  _poller->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel* channel)
{
  assert(channel->ownerLoop() == this);
  assertInLoopThread();
  return _poller->hasChannel(channel);
}

void EventLoop::abortNotInLoopThread()
{
}

void EventLoop::wakeup()
{
  uint64_t one = 1;
  ssize_t n = ::write(_wakeupFd, &one, sizeof one);
  if (n != sizeof one) {
    LOG_ERROR<<"EventLoop::wakeup() writes "<<n<<" bytes instead of 8";
  }
}

void EventLoop::handleRead()
{
  uint64_t one = 1;
  ssize_t n = ::read(_wakeupFd, &one, sizeof one);
  if (n != sizeof one) {
    LOG_ERROR<<"EventLoop::wakeup() reads "<<n<<" bytes instead of 8";
  }
}

void EventLoop::doPendingFunctors()
{
  std::vector<Functor> functors;
  _callingPendingFunctors = true;

  {
  ScopeMutex lock(&_mutex);
  functors.swap(_pendingFunctors);
  }

  for (size_t i = 0; i < functors.size(); ++i)
  {
    functors[i]();
  }
  _callingPendingFunctors = false;
}

void EventLoop::printActiveChannels() const
{
  for (ChannelList::const_iterator it = _activeChannels.begin();
      it != _activeChannels.end(); ++it)
  {
    const Channel* ch = *it;
    LOG_TRACE<<"{ "<<ch->reventsToString()<<" }";
  }
}


