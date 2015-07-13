#ifndef THREAD_H_
#define THREAD_H_
#include <stdint.h>
#include <pthread.h>
#include "Atomic.h"

namespace walle{
namespace sys {

class Thread {
public:
  /**
   * Default constructor.
   */
  explicit Thread();
  /**
   * Destructor.
   */
  virtual ~Thread();
  /**
   * Perform the concrete process.
   */
  virtual void run() = 0;
  /**
   * Start the thread.
   */
  bool start();
  bool running()
  { return _alive;}
  /**
   * Wait for the thread to finish.
   */
  bool join();
  /**
   * Put the thread in the detached state.
   */
  bool detach();
  /**
   * Terminate the running thread.
   */
  static void exit();
  /**
   * Yield the processor from the current thread.
   */
  static void yield();
  /**
   * Suspend execution of the current thread.
   * @param sec the interval of the suspension in seconds.
   * @return true on success, or false on failure.
   */
  static bool sleep(int64_t usec);
  int    seq();   
  /**
   * Get the hash value of the current thread.
   * @return the hash value of the current thread.
   */
  static int64_t hash();
private:
  /** Dummy constructor to forbid the use. */
  Thread(const Thread&);
  /** Dummy Operator to forbid the use. */
  Thread& operator =(const Thread&);
  /** Opaque pointer. */
protected:
  ::pthread_t _th;			
  bool        _alive;
  int         _seq;
  static AtomicInt32 _numCreated;
};

}
}
#endif /* THREAD_H_ */
