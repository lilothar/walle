#ifndef WALLE_SPINLOCK_H_
#define WALLE_SPINLOCK_H_
#include <pthread.h>
#include <assert.h>
#include <stdint.h>


namespace walle{
namespace sys {
class SpinLock{
public:
      /**
       * Default constructor.
       */
     explicit SpinLock();
      /**
       * Destructor.
       */
     ~SpinLock();
      /**
       * Get the lock.
       */
     void lock();
      /**
       * Try to get the lock.
       * @return true on success, or false on failure.
       */
     bool lock_try();
      /**
       * Release the lock.
       */
     void unlock();
private:
      pthread_spinlock_t _spin;

};
}
}
#endif
