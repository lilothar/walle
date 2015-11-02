#include <walle/net/Eventloopthreadpool.h>

#include <walle/net/Eventloop.h>
#include <walle/net/Eventloopthread.h>

#include <boost/bind.hpp>

using namespace walle::sys;
namespace walle {
namespace net {

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop)
  : _baseLoop(baseLoop),
    _started(false),
    _numThreads(0),
    _next(0)
{
}

EventLoopThreadPool::~EventLoopThreadPool()
{
  // Don't delete loop, it's stack variable
}

void EventLoopThreadPool::start(const ThreadInitCallback& cb)
{
  assert(!_started);
  _baseLoop->assertInLoopThread();

  _started = true;

  for (int i = 0; i < _numThreads; ++i)
  {
    EventLoopThread* t = new EventLoopThread(cb);
    _threads.push_back(t);
    _loops.push_back(t->startLoop());
  }
  if (_numThreads == 0 && cb)
  {
    cb(_baseLoop);
  }
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
  _baseLoop->assertInLoopThread();
  assert(_started);
  EventLoop* loop = _baseLoop;

  if (!_loops.empty())
  {
    // round-robin
    loop = _loops[_next];
    ++_next;
    if (_next >= static_cast<int>(_loops.size()))
    {
      _next = 0;
    }
  }
  return loop;
}

EventLoop* EventLoopThreadPool::getLoopForHash(size_t hashCode)
{
  _baseLoop->assertInLoopThread();
  EventLoop* loop = _baseLoop;

  if (!_loops.empty())
  {
    loop = _loops[hashCode % _loops.size()];
  }
  return loop;
}

std::vector<EventLoop*> EventLoopThreadPool::getAllLoops()
{
  _baseLoop->assertInLoopThread();
  assert(_started);
  if (_loops.empty())
  {
    return std::vector<EventLoop*>(1, _baseLoop);
  }
  else
  {
    return _loops;
  }
}
}
}

