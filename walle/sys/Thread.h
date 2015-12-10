#ifndef WALLE_THREAD_THREAD_H_
#define WALLE_THREAD_THREAD_H_

#include <walle/sys/Atomic.h>
#include <walle/algo/functional.h>
#include <walle/algo/noncopyable.h>
#include <walle/algo/memory.h>
#include <pthread.h>
#include <string>

namespace walle {
namespace sys {
    
class Thread : std::noncopyable
{
 public:
  typedef std::function<void ()> ThreadFunc;

  explicit Thread(const ThreadFunc&, const std::string& name = std::string());

  ~Thread();

  void start();
  int join(); // return pthread_join()

  bool started() const { return _started; }
  // pthread_t pthreadId() const { return _pthreadId; }
  pid_t tid() const { return *_tid; }
  const std::string& name() const { return _name; }

   bool detach();
   /**   * Terminate the running thread.   */  

   static void exit();
   /**   * Yield the processor from the current thread.   */  
   static void yield();
   /**   * Suspend execution of the current thread.
   * @param sec the interval of the suspension in seconds.
   * @return true on success, or false on failure.
   */  
   static bool sleep(int64_t usec);
   int    seq(); 
   /**   * Get the hash value of the current thread.
   * @return the hash value of the current thread.
   */  

  static int64_t hash();
  static int numCreated() { return _numCreated.get(); }

 private:
  void setDefaultName();

  bool       _started;
  bool       _joined;
  pthread_t  _pthreadId;
  std::shared_ptr<pid_t> _tid;
  ThreadFunc _func;
  std::string     _name;
  int32_t         _seq;
  static AtomicInt32 _numCreated;
};

}
}
#endif
