#ifndef WALLE_NET_EVENTLOOPTHREAD_H_
#define WALLE_NET_EVENTLOOPTHREAD_H_
#include <walle/sys/wallesys.h>

#include <walle/algo/functional.h>
#include <walle/algo//noncopyable.h>
using namespace walle::sys;
namespace walle {
namespace net{

class EventLoop;

class EventLoopThread : public Thread
{
 public:
  typedef std::function<void(EventLoop*)> ThreadInitCallback;

  EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback());
  ~EventLoopThread();
  EventLoop* startLoop();
  virtual void run();

 private:
  

  EventLoop* _loop;
  bool _exiting;
  Mutex _mutex;
  Cond _cond;
  ThreadInitCallback _callback;
};

}
}
#endif