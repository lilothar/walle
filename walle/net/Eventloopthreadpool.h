#ifndef DYLIN_EVENTLOOPTHREADPOOL_H_
#define DYLIN_EVENTLOOPTHREADPOOL_H_

#include <vector>
#include <boost/function.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace walle {
namespace net{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool
{
 public:
  typedef boost::function<void(EventLoop*)> ThreadInitCallback;

  EventLoopThreadPool(EventLoop* baseLoop);
  ~EventLoopThreadPool();
  void setThreadNum(int numThreads) { _numThreads = numThreads; }
  void start(const ThreadInitCallback& cb = ThreadInitCallback());

  // valid after calling start()
  /// round-robin
  EventLoop* getNextLoop();

  /// with the same hash code, it will always return the same EventLoop
  EventLoop* getLoopForHash(size_t hashCode);

  std::vector<EventLoop*> getAllLoops();

  bool started() const
  { return _started; }

 private:

  EventLoop* _baseLoop;
  bool _started;
  int _numThreads;
  int _next;
  boost::ptr_vector<EventLoopThread> _threads;
  std::vector<EventLoop*> _loops;
};
}
}
#endif