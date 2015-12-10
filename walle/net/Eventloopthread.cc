#include <walle/net/Eventloopthread.h>
#include <walle/net/Eventloop.h>

using namespace walle::sys;

namespace walle {
namespace net {
	
EventLoopThread::EventLoopThread(const ThreadInitCallback& cb, const string & name)
  : _loop(NULL),
    _exiting(false), // FIXME: number it
    _mutex(),
    _cond(),
    _thread(std::bind(&EventLoopThread::threadFunc, this), name),
    _callback(cb)
{
}

EventLoopThread::~EventLoopThread()
{
  _exiting = true;
  if (_loop != NULL) // not 100% race-free, eg. threadFunc could be running callback_.
  {
    // still a tiny chance to call destructed object, if threadFunc exits just now.
    // but when EventLoopThread destructs, usually programming is exiting anyway.
    _loop->quit();
    _thread.join();
  }
}

EventLoop* EventLoopThread::startLoop()
{
  assert(!_thread.started());

  _thread.start();

  {
    ScopeMutex lock(&_mutex);
    while (_loop == NULL)
    {
      _cond.wait(&_mutex);
    }
  }

  return _loop;
}

void EventLoopThread::threadFunc()
{
  EventLoop loop;

  if (_callback)
  {
    _callback(&loop);
  }

  {
    ScopeMutex lock(&_mutex);
    _loop = &loop;
    _cond.signal();
  }

  loop.loop();
  //assert(exiting_);
  _loop = NULL;
}
}
}
