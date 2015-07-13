
#ifndef SCOPELOCK_H_
#define SCOPELOCK_H_
#include <assert.h>
#include <walle/sys/Mutex.h>
#include <walle/sys/SpinLock.h>

namespace walle {
namespace sys {

template <typename LOCK>

class ScopeLock {
public:
    explicit ScopeLock(LOCK* lock)
    {
        _lock = lock;
        if (_lock)
            _lock->lock();
    }
    ~ScopeLock()
    {
      if (_lock)
        _lock->unlock();  
    }
private:
    LOCK* _lock;
};

typedef ScopeLock<Mutex> ScopeMutex;
typedef ScopeLock<SpinLock> ScopeSpinlock;
}
}
#endif
