#ifndef WALLE_COND_H_
#define WALLE_COND_H_
#include <stdint.h>
#include <pthread.h>

namespace walle {
namespace sys {

class Mutex;
class Cond{
public:
      /**
       * Default constructor.
       */
    explicit Cond();
      /**
       * Destructor.
       */
     ~Cond();
      /**
       * Wait for the signal.
       * @param mutex a locked mutex.
       */
     void wait(Mutex* mutex);
      /**
       * Wait for the signal.
       * @param mutex a locked mutex.
       * @param sec the interval of the suspension in seconds.
       * @return true on catched signal, or false on timeout.
       */
     bool wait(Mutex* mutex, int32_t usec);
      /**
       * Send the wake-up signal to another waiting thread.
       * @note The mutex used for the wait method should be locked by the caller.
       */
     void signal();
      /**
       * Send the wake-up signal to all waiting threads.
       * @note The mutex used for the wait method should be locked by the caller.
       */
     void broadcast();
private:
     pthread_cond_t _cond;

};
}
}
#endif
