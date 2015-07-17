#include <walle/net/Channel.h>
#include <walle/net/Poller.h>
//#include <walle/net/Socket.h>
#include <walle/net/Timer.h>
#include <walle/net/EventLoop.h>
#include <pthread.h>
#include <boost/bind.hpp>
#include <walle/net/Waker.h>
#include <signal.h>


using namespace walle::sys;

namespace {
	using namespace walle::net;
__thread walle::net::EventLoop* t_loopInThisThread = 0;

const int kPollTimeMs = 10000;

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

namespace walle {
namespace net {
EventLoop::EventLoop()
  : _looping(false),
    _quit(false),
    _eventHandling(false),
    _callingPendingFunctors(false),
    _iteration(0),
    _threadId(walle::LocalThread::tid()),
    _poller(new Poller(this)),
    _timer(boost::shared_ptr<Timer>(new Timer(this))),
    _timerChannel(boost::shared_ptr<Timer>(_timer)->channel()),
    _waker(boost::shared_ptr<Waker>(new Waker(this))),
    _wakerChannel(boost::shared_ptr<Waker>(_waker)->channel()),
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
	  boost::shared_ptr<Timer>(_timer)->start();
	  boost::shared_ptr<Waker>(_waker)->start();
}

EventLoop::~EventLoop()
{
  boost::shared_ptr<Timer>(_timer)->stop();
  boost::shared_ptr<Waker>(_waker)->stop();
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
  return boost::shared_ptr<Timer>(_timer)->addTimer(cb, time, 0);
}

TimerId EventLoop::runAfter(int64_t delay, const TimerCallback& cb)
{
  Time time(Time::now() + delay);
  return runAt(time, cb);
}

TimerId EventLoop::runEvery(int64_t interval, const TimerCallback& cb)
{
  Time time(Time::now() + interval);
  return boost::shared_ptr<Timer>(_timer)->addTimer(cb, time, interval);
}

void EventLoop::cancel(TimerId timerId)
{
  return boost::shared_ptr<Timer>(_timer)->cancel(timerId);
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
	 boost::shared_ptr<Waker>(_waker)->wakeUp();
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
}
}

