#ifndef WALLE_THREAD_THREADPOOL_H_
#define WALLE_THREAD_THREADPOOL_H_

#include <walle/sys/Cond.h>
#include <walle/sys/Mutex.h>
#include <walle/sys/Thread.h>

#include <walle/algo/functional.h>
#include <walle/algo/noncopyable.h>
#include <deque>
#include <string>
#include <vector>

using std::string;

namespace walle  {
namespace sys {

class ThreadPool : std::noncopyable {
 public:
  typedef std::function<void ()> Task;

  explicit ThreadPool(const string& nameArg = string("ThreadPool"));
  ~ThreadPool();

  // Must be called before start().
  void setMaxQueueSize(int maxSize) { _maxQueueSize = maxSize; }
  void setThreadInitCallback(const Task& cb)
  { _threadInitCallback = cb; }

  void start(int numThreads);
  void stop();

  const string& name() const
  { return _name; }

  size_t queueSize() const;

  // Could block if maxQueueSize > 0
  void run(const Task& f);
 private:
  bool isFull() const;
  void runInThread();
  Task take();

  mutable Mutex _mutex;
  Cond _notEmpty;
  Cond _notFull;
  string _name;
  Task _threadInitCallback;
  std::vector<walle::thread::Thread*> _threads;
  std::deque<Task> _queue;
  size_t _maxQueueSize;
  bool _running;
};

}
}
#endif
