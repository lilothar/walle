#ifndef WALLE_MUTEX_H_
#define WALLE_MUTEX_H_
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
#include <cmath>

namespace walle{
namespace sys {
  enum Type {
    FAST,                ///< no operation
    ERRORCHECK,          ///< check error
    RECURSIVE            ///< allow recursive locking
  };
class Mutex{

public:
      friend class Cond;
public:

  explicit Mutex();

  explicit Mutex(Type type);

  ~Mutex();

  void lock();

  bool lock_try();

  bool lock_try(int64_t usec);

  void unlock();
private:
  pthread_mutex_t _mutex;
};
}
}
#endif