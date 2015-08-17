#include <walle/store/thread.h>
#include <walle/store/myconf.h>

namespace walle {
namespace store {



const uint32_t LOCKBUSYLOOP = 8192;      ///< threshold of busy loop and sleep for locking
const size_t LOCKSEMNUM = 256;           ///< number of semaphores for locking


/**
 * Thread internal.
 */
struct ThreadCore {
  ::pthread_t th;                        ///< identifier
  bool alive;                            ///< alive flag
};


/**
 * CondVar internal.
 */
struct CondVarCore {
  ::pthread_cond_t cond;                 ///< condition
};

static void* threadrun(void* arg);


/**
 * Default constructor.
 */
Thread::Thread() : opq_(NULL) {
  assert(true);
  ThreadCore* core = new ThreadCore;
  core->alive = false;
  opq_ = (void*)core;
}


/**
 * Destructor.
 */
Thread::~Thread() {
  assert(true);
  ThreadCore* core = (ThreadCore*)opq_;
  if (core->alive) join();
  delete core;
}


/**
 * Start the thread.
 */
void Thread::start() {
  assert(true);
  ThreadCore* core = (ThreadCore*)opq_;
  if (core->alive) throw std::invalid_argument("already started");
  if (::pthread_create(&core->th, NULL, threadrun, this) != 0)
    throw std::runtime_error("pthread_create");
  core->alive = true;
}


/**
 * Wait for the thread to finish.
 */
void Thread::join() {
  assert(true);
  ThreadCore* core = (ThreadCore*)opq_;
  if (!core->alive) throw std::invalid_argument("not alive");
  core->alive = false;
  if (::pthread_join(core->th, NULL) != 0) throw std::runtime_error("pthread_join");
}


/**
 * Put the thread in the detached state.
 */
void Thread::detach() {
  assert(true);
  ThreadCore* core = (ThreadCore*)opq_;
  if (!core->alive) throw std::invalid_argument("not alive");
  core->alive = false;
  if (::pthread_detach(core->th) != 0) throw std::runtime_error("pthread_detach");
}


/**
 * Terminate the running thread.
 */
void Thread::exit() {
  assert(true);
  ::pthread_exit(NULL);
}


/**
 * Yield the processor from the current thread.
 */
void Thread::yield() {
  assert(true);
  ::sched_yield();
}


/**
 * Chill the processor by suspending execution for a quick moment.
 */
void Thread::chill() {
  assert(true);
  struct ::timespec req;
  req.tv_sec = 0;
  req.tv_nsec = 21 * 1000 * 1000;
  ::nanosleep(&req, NULL);
}



/**
 * Suspend execution of the current thread.
 */
bool Thread::sleep(double sec) {
  assert(sec >= 0.0);
  if (sec <= 0.0) {
    yield();
    return true;
  }
  if (sec > INT32MAX) sec = INT32MAX;
  double integ, fract;
  fract = std::modf(sec, &integ);
  struct ::timespec req, rem;
  req.tv_sec = (time_t)integ;
  req.tv_nsec = (long)(fract * 999999000);
  while (::nanosleep(&req, &rem) != 0) {
    if (errno != EINTR) return false;
    req = rem;
  }
  return true;
}


/**
 * Get the hash value of the current thread.
 */
int64_t Thread::hash() {
  assert(true);
  pthread_t tid = pthread_self();
  int64_t num;
  if (sizeof(tid) == sizeof(num)) {
    std::memcpy(&num, &tid, sizeof(num));
  } else if (sizeof(tid) == sizeof(int32_t)) {
    uint32_t inum;
    std::memcpy(&inum, &tid, sizeof(inum));
    num = inum;
  } else {
    num = hashmurmur(&tid, sizeof(tid));
  }
  return num & INT64MAX;
}


/**
 * Call the running thread.
 */
static void* threadrun(void* arg) {
  assert(true);
  Thread* thread = (Thread*)arg;
  thread->run();
  return NULL;
}


/**
 * Default constructor.
 */
Mutex::Mutex() : opq_(NULL) {
  assert(true);
  ::pthread_mutex_t* mutex = new ::pthread_mutex_t;
  if (::pthread_mutex_init(mutex, NULL) != 0) throw std::runtime_error("pthread_mutex_init");
  opq_ = (void*)mutex;
}


/**
 * Constructor with the specifications.
 */
Mutex::Mutex(Type type) {
  assert(true);
  ::pthread_mutexattr_t attr;
  if (::pthread_mutexattr_init(&attr) != 0) throw std::runtime_error("pthread_mutexattr_init");
  switch (type) {
    case FAST: {
      break;
    }
    case ERRORCHECK: {
      if (::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK) != 0)
        throw std::runtime_error("pthread_mutexattr_settype");
      break;
    }
    case RECURSIVE: {
      if (::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0)
        throw std::runtime_error("pthread_mutexattr_settype");
      break;
    }
  }
  ::pthread_mutex_t* mutex = new ::pthread_mutex_t;
  if (::pthread_mutex_init(mutex, &attr) != 0) throw std::runtime_error("pthread_mutex_init");
  ::pthread_mutexattr_destroy(&attr);
  opq_ = (void*)mutex;
}


/**
 * Destructor.
 */
Mutex::~Mutex() {
  assert(true);
  ::pthread_mutex_t* mutex = (::pthread_mutex_t*)opq_;
  ::pthread_mutex_destroy(mutex);
  delete mutex;
}


/**
 * Get the lock.
 */
void Mutex::lock() {
  assert(true);
  ::pthread_mutex_t* mutex = (::pthread_mutex_t*)opq_;
  if (::pthread_mutex_lock(mutex) != 0) throw std::runtime_error("pthread_mutex_lock");
}


/**
 * Try to get the lock.
 */
bool Mutex::lock_try() {
  assert(true);
  ::pthread_mutex_t* mutex = (::pthread_mutex_t*)opq_;
  int32_t ecode = ::pthread_mutex_trylock(mutex);
  if (ecode == 0) return true;
  if (ecode != EBUSY) throw std::runtime_error("pthread_mutex_trylock");
  return false;
}


/**
 * Try to get the lock.
 */
bool Mutex::lock_try(double sec) {
  assert(sec >= 0.0);
  ::pthread_mutex_t* mutex = (::pthread_mutex_t*)opq_;
  struct ::timeval tv;
  struct ::timespec ts;
  if (::gettimeofday(&tv, NULL) == 0) {
    double integ;
    double fract = std::modf(sec, &integ);
    ts.tv_sec = tv.tv_sec + (time_t)integ;
    ts.tv_nsec = (long)(tv.tv_usec * 1000.0 + fract * 999999000);
    if (ts.tv_nsec >= 1000000000) {
      ts.tv_nsec -= 1000000000;
      ts.tv_sec++;
    }
  } else {
    ts.tv_sec = std::time(NULL) + 1;
    ts.tv_nsec = 0;
  }
  int32_t ecode = ::pthread_mutex_timedlock(mutex, &ts);
  if (ecode == 0) return true;
  if (ecode != ETIMEDOUT) throw std::runtime_error("pthread_mutex_timedlock");
  return false;
}


/**
 * Release the lock.
 */
void Mutex::unlock() {
  assert(true);
  ::pthread_mutex_t* mutex = (::pthread_mutex_t*)opq_;
  if (::pthread_mutex_unlock(mutex) != 0) throw std::runtime_error("pthread_mutex_unlock");
}


/**
 * SlottedMutex internal.
 */
struct SlottedMutexCore {
  ::pthread_mutex_t* mutexes;            ///< primitives
  size_t slotnum;                        ///< number of slots
};


/**
 * Constructor.
 */
SlottedMutex::SlottedMutex(size_t slotnum) : opq_(NULL) {
  assert(true);
  SlottedMutexCore* core = new SlottedMutexCore;
  ::pthread_mutex_t* mutexes = new ::pthread_mutex_t[slotnum];
  for (size_t i = 0; i < slotnum; i++) {
    if (::pthread_mutex_init(mutexes + i, NULL) != 0)
      throw std::runtime_error("pthread_mutex_init");
  }
  core->mutexes = mutexes;
  core->slotnum = slotnum;
  opq_ = (void*)core;
}


/**
 * Destructor.
 */
SlottedMutex::~SlottedMutex() {
  assert(true);
  SlottedMutexCore* core = (SlottedMutexCore*)opq_;
  ::pthread_mutex_t* mutexes = core->mutexes;
  size_t slotnum = core->slotnum;
  for (size_t i = 0; i < slotnum; i++) {
    ::pthread_mutex_destroy(mutexes + i);
  }
  delete[] mutexes;
  delete core;
}


/**
 * Get the lock of a slot.
 */
void SlottedMutex::lock(size_t idx) {
  assert(true);
  SlottedMutexCore* core = (SlottedMutexCore*)opq_;
  if (::pthread_mutex_lock(core->mutexes + idx) != 0)
    throw std::runtime_error("pthread_mutex_lock");
}


/**
 * Release the lock of a slot.
 */
void SlottedMutex::unlock(size_t idx) {
  assert(true);
  SlottedMutexCore* core = (SlottedMutexCore*)opq_;
  if (::pthread_mutex_unlock(core->mutexes + idx) != 0)
    throw std::runtime_error("pthread_mutex_unlock");
}


/**
 * Get the locks of all slots.
 */
void SlottedMutex::lock_all() {
  assert(true);
  SlottedMutexCore* core = (SlottedMutexCore*)opq_;
  ::pthread_mutex_t* mutexes = core->mutexes;
  size_t slotnum = core->slotnum;
  for (size_t i = 0; i < slotnum; i++) {
    if (::pthread_mutex_lock(mutexes + i) != 0)
      throw std::runtime_error("pthread_mutex_lock");
  }
}


/**
 * Release the locks of all slots.
 */
void SlottedMutex::unlock_all() {
  assert(true);
  SlottedMutexCore* core = (SlottedMutexCore*)opq_;
  ::pthread_mutex_t* mutexes = core->mutexes;
  size_t slotnum = core->slotnum;
  for (size_t i = 0; i < slotnum; i++) {
    if (::pthread_mutex_unlock(mutexes + i) != 0)
      throw std::runtime_error("pthread_mutex_unlock");
  }
}


/**
 * Default constructor.
 */
SpinLock::SpinLock() : opq_(NULL) {
  assert(true);
  ::pthread_spinlock_t* spin = new ::pthread_spinlock_t;
  if (::pthread_spin_init(spin, PTHREAD_PROCESS_PRIVATE) != 0)
    throw std::runtime_error("pthread_spin_init");
  opq_ = (void*)spin;
}


/**
 * Destructor.
 */
SpinLock::~SpinLock() {
  assert(true);
  ::pthread_spinlock_t* spin = (::pthread_spinlock_t*)opq_;
  ::pthread_spin_destroy(spin);
  delete spin;
}


/**
 * Get the lock.
 */
void SpinLock::lock() {
  assert(true);
  ::pthread_spinlock_t* spin = (::pthread_spinlock_t*)opq_;
  if (::pthread_spin_lock(spin) != 0) throw std::runtime_error("pthread_spin_lock");
}


/**
 * Try to get the lock.
 */
bool SpinLock::lock_try() {
  assert(true);
  ::pthread_spinlock_t* spin = (::pthread_spinlock_t*)opq_;
  int32_t ecode = ::pthread_spin_trylock(spin);
  if (ecode == 0) return true;
  if (ecode != EBUSY) throw std::runtime_error("pthread_spin_trylock");
  return false;
}


/**
 * Release the lock.
 */
void SpinLock::unlock() {
  assert(true);
  ::pthread_spinlock_t* spin = (::pthread_spinlock_t*)opq_;
  if (::pthread_spin_unlock(spin) != 0) throw std::runtime_error("pthread_spin_unlock");
}


/**
 * SlottedSpinLock internal.
 */
struct SlottedSpinLockCore {
  ::pthread_spinlock_t* spins;           ///< primitives
  size_t slotnum;                        ///< number of slots
};


/**
 * Constructor.
 */
SlottedSpinLock::SlottedSpinLock(size_t slotnum) : opq_(NULL) {
  assert(true);
  SlottedSpinLockCore* core = new SlottedSpinLockCore;
  ::pthread_spinlock_t* spins = new ::pthread_spinlock_t[slotnum];
  for (size_t i = 0; i < slotnum; i++) {
    if (::pthread_spin_init(spins + i, PTHREAD_PROCESS_PRIVATE) != 0)
      throw std::runtime_error("pthread_spin_init");
  }
  core->spins = spins;
  core->slotnum = slotnum;
  opq_ = (void*)core;
}


/**
 * Destructor.
 */
SlottedSpinLock::~SlottedSpinLock() {
  assert(true);
  SlottedSpinLockCore* core = (SlottedSpinLockCore*)opq_;
  ::pthread_spinlock_t* spins = core->spins;
  size_t slotnum = core->slotnum;
  for (size_t i = 0; i < slotnum; i++) {
    ::pthread_spin_destroy(spins + i);
  }
  delete[] spins;
  delete core;
}


/**
 * Get the lock of a slot.
 */
void SlottedSpinLock::lock(size_t idx) {
  assert(true);
  SlottedSpinLockCore* core = (SlottedSpinLockCore*)opq_;
  if (::pthread_spin_lock(core->spins + idx) != 0)
    throw std::runtime_error("pthread_spin_lock");
}


/**
 * Release the lock of a slot.
 */
void SlottedSpinLock::unlock(size_t idx) {
  assert(true);
  SlottedSpinLockCore* core = (SlottedSpinLockCore*)opq_;
  if (::pthread_spin_unlock(core->spins + idx) != 0)
    throw std::runtime_error("pthread_spin_unlock");
}


/**
 * Get the locks of all slots.
 */
void SlottedSpinLock::lock_all() {
  assert(true);
  SlottedSpinLockCore* core = (SlottedSpinLockCore*)opq_;
  ::pthread_spinlock_t* spins = core->spins;
  size_t slotnum = core->slotnum;
  for (size_t i = 0; i < slotnum; i++) {
    if (::pthread_spin_lock(spins + i) != 0)
      throw std::runtime_error("pthread_spin_lock");
  }
}


/**
 * Release the locks of all slots.
 */
void SlottedSpinLock::unlock_all() {
  assert(true);
  SlottedSpinLockCore* core = (SlottedSpinLockCore*)opq_;
  ::pthread_spinlock_t* spins = core->spins;
  size_t slotnum = core->slotnum;
  for (size_t i = 0; i < slotnum; i++) {
    if (::pthread_spin_unlock(spins + i) != 0)
      throw std::runtime_error("pthread_spin_unlock");
  }
}


/**
 * Default constructor.
 */
RWLock::RWLock() : opq_(NULL) {
  assert(true);
  ::pthread_rwlock_t* rwlock = new ::pthread_rwlock_t;
  if (::pthread_rwlock_init(rwlock, NULL) != 0) throw std::runtime_error("pthread_rwlock_init");
  opq_ = (void*)rwlock;
}


/**
 * Destructor.
 */
RWLock::~RWLock() {
  assert(true);
  ::pthread_rwlock_t* rwlock = (::pthread_rwlock_t*)opq_;
  ::pthread_rwlock_destroy(rwlock);
  delete rwlock;
}


/**
 * Get the writer lock.
 */
void RWLock::lock_writer() {
  assert(true);
  ::pthread_rwlock_t* rwlock = (::pthread_rwlock_t*)opq_;
  if (::pthread_rwlock_wrlock(rwlock) != 0) throw std::runtime_error("pthread_rwlock_lock");
}


/**
 * Try to get the writer lock.
 */
bool RWLock::lock_writer_try() {
  assert(true);
  ::pthread_rwlock_t* rwlock = (::pthread_rwlock_t*)opq_;
  int32_t ecode = ::pthread_rwlock_trywrlock(rwlock);
  if (ecode == 0) return true;
  if (ecode != EBUSY) throw std::runtime_error("pthread_rwlock_trylock");
  return false;
}


/**
 * Get a reader lock.
 */
void RWLock::lock_reader() {
  assert(true);
  ::pthread_rwlock_t* rwlock = (::pthread_rwlock_t*)opq_;
  if (::pthread_rwlock_rdlock(rwlock) != 0) throw std::runtime_error("pthread_rwlock_lock");
}


/**
 * Try to get a reader lock.
 */
bool RWLock::lock_reader_try() {
  assert(true);
  ::pthread_rwlock_t* rwlock = (::pthread_rwlock_t*)opq_;
  int32_t ecode = ::pthread_rwlock_tryrdlock(rwlock);
  if (ecode == 0) return true;
  if (ecode != EBUSY) throw std::runtime_error("pthread_rwlock_trylock");
  return false;
}


/**
 * Release the lock.
 */
void RWLock::unlock() {
  assert(true);
  ::pthread_rwlock_t* rwlock = (::pthread_rwlock_t*)opq_;
  if (::pthread_rwlock_unlock(rwlock) != 0) throw std::runtime_error("pthread_rwlock_unlock");
}


/**
 * SlottedRWLock internal.
 */
struct SlottedRWLockCore {
  ::pthread_rwlock_t* rwlocks;           ///< primitives
  size_t slotnum;                        ///< number of slots
};


/**
 * Constructor.
 */
SlottedRWLock::SlottedRWLock(size_t slotnum) : opq_(NULL) {
  assert(true);
  SlottedRWLockCore* core = new SlottedRWLockCore;
  ::pthread_rwlock_t* rwlocks = new ::pthread_rwlock_t[slotnum];
  for (size_t i = 0; i < slotnum; i++) {
    if (::pthread_rwlock_init(rwlocks + i, NULL) != 0)
      throw std::runtime_error("pthread_rwlock_init");
  }
  core->rwlocks = rwlocks;
  core->slotnum = slotnum;
  opq_ = (void*)core;
}


/**
 * Destructor.
 */
SlottedRWLock::~SlottedRWLock() {
  assert(true);
  SlottedRWLockCore* core = (SlottedRWLockCore*)opq_;
  ::pthread_rwlock_t* rwlocks = core->rwlocks;
  size_t slotnum = core->slotnum;
  for (size_t i = 0; i < slotnum; i++) {
    ::pthread_rwlock_destroy(rwlocks + i);
  }
  delete[] rwlocks;
  delete core;
}


/**
 * Get the writer lock of a slot.
 */
void SlottedRWLock::lock_writer(size_t idx) {
  assert(true);
  SlottedRWLockCore* core = (SlottedRWLockCore*)opq_;
  if (::pthread_rwlock_wrlock(core->rwlocks + idx) != 0)
    throw std::runtime_error("pthread_rwlock_wrlock");
}


/**
 * Get the reader lock of a slot.
 */
void SlottedRWLock::lock_reader(size_t idx) {
  assert(true);
  SlottedRWLockCore* core = (SlottedRWLockCore*)opq_;
  if (::pthread_rwlock_rdlock(core->rwlocks + idx) != 0)
    throw std::runtime_error("pthread_rwlock_rdlock");
}


/**
 * Release the lock of a slot.
 */
void SlottedRWLock::unlock(size_t idx) {
  assert(true);
  SlottedRWLockCore* core = (SlottedRWLockCore*)opq_;
  if (::pthread_rwlock_unlock(core->rwlocks + idx) != 0)
    throw std::runtime_error("pthread_rwlock_unlock");
}


/**
 * Get the writer locks of all slots.
 */
void SlottedRWLock::lock_writer_all() {
  assert(true);
  SlottedRWLockCore* core = (SlottedRWLockCore*)opq_;
  ::pthread_rwlock_t* rwlocks = core->rwlocks;
  size_t slotnum = core->slotnum;
  for (size_t i = 0; i < slotnum; i++) {
    if (::pthread_rwlock_wrlock(rwlocks + i) != 0)
      throw std::runtime_error("pthread_rwlock_wrlock");
  }
}


/**
 * Get the reader locks of all slots.
 */
void SlottedRWLock::lock_reader_all() {
  assert(true);
  SlottedRWLockCore* core = (SlottedRWLockCore*)opq_;
  ::pthread_rwlock_t* rwlocks = core->rwlocks;
  size_t slotnum = core->slotnum;
  for (size_t i = 0; i < slotnum; i++) {
    if (::pthread_rwlock_rdlock(rwlocks + i) != 0)
      throw std::runtime_error("pthread_rwlock_rdlock");
  }
 }


/**
 * Release the locks of all slots.
 */
void SlottedRWLock::unlock_all() {
   assert(true);
  SlottedRWLockCore* core = (SlottedRWLockCore*)opq_;
  ::pthread_rwlock_t* rwlocks = core->rwlocks;
  size_t slotnum = core->slotnum;
  for (size_t i = 0; i < slotnum; i++) {
    if (::pthread_rwlock_unlock(rwlocks + i) != 0)
      throw std::runtime_error("pthread_rwlock_unlock");
  }
 }


/**
 * SpinRWLock internal.
 */
struct SpinRWLockCore { 
  ::pthread_spinlock_t sem;              ///< semaphore
  uint32_t cnt;             
};


/**
 * Lock the semephore of SpinRWLock.
 * @param core the internal fields.
 */
static void spinrwlocklock(SpinRWLockCore* core);


/**
 * Unlock the semephore of SpinRWLock.
 * @param core the internal fields.
 */
static void spinrwlockunlock(SpinRWLockCore* core);


/**
 * Default constructor.
 */
SpinRWLock::SpinRWLock() : opq_(NULL) {
   assert(true);
  SpinRWLockCore* core = new SpinRWLockCore;
  if (::pthread_spin_init(&core->sem, PTHREAD_PROCESS_PRIVATE) != 0)
    throw std::runtime_error("pthread_spin_init");
  core->cnt = 0;
  opq_ = (void*)core;
 }


/**
 * Destructor.
 */
SpinRWLock::~SpinRWLock() {
   assert(true);
  SpinRWLockCore* core = (SpinRWLockCore*)opq_;
  ::pthread_spin_destroy(&core->sem);
  delete core;
 }


/**
 * Get the writer lock.
 */
void SpinRWLock::lock_writer() {
  assert(true);
  SpinRWLockCore* core = (SpinRWLockCore*)opq_;
  spinrwlocklock(core);
  uint32_t wcnt = 0;
  while (core->cnt > 0) {
    spinrwlockunlock(core);
    if (wcnt >= LOCKBUSYLOOP) {
      Thread::chill();
    } else {
      Thread::yield();
      wcnt++;
    }
    spinrwlocklock(core);
  }
  core->cnt = INT32MAX;
  spinrwlockunlock(core);
}


/**
 * Try to get the writer lock.
 */
bool SpinRWLock::lock_writer_try() {
  assert(true);
  SpinRWLockCore* core = (SpinRWLockCore*)opq_;
  spinrwlocklock(core);
  if (core->cnt > 0) {
    spinrwlockunlock(core);
    return false;
  }
  core->cnt = INT32MAX;
  spinrwlockunlock(core);
  return true;
}


/**
 * Get a reader lock.
 */
void SpinRWLock::lock_reader() {
  assert(true);
  SpinRWLockCore* core = (SpinRWLockCore*)opq_;
  spinrwlocklock(core);
  uint32_t wcnt = 0;
  while (core->cnt >= (uint32_t)INT32MAX) {
    spinrwlockunlock(core);
    if (wcnt >= LOCKBUSYLOOP) {
      Thread::chill();
    } else {
      Thread::yield();
      wcnt++;
    }
    spinrwlocklock(core);
  }
  core->cnt++;
  spinrwlockunlock(core);
}


/**
 * Try to get a reader lock.
 */
bool SpinRWLock::lock_reader_try() {
  assert(true);
  SpinRWLockCore* core = (SpinRWLockCore*)opq_;
  spinrwlocklock(core);
  if (core->cnt >= (uint32_t)INT32MAX) {
    spinrwlockunlock(core);
    return false;
  }
  core->cnt++;
  spinrwlockunlock(core);
  return true;
}


/**
 * Release the lock.
 */
void SpinRWLock::unlock() {
  assert(true);
  SpinRWLockCore* core = (SpinRWLockCore*)opq_;
  spinrwlocklock(core);
  if (core->cnt >= (uint32_t)INT32MAX) {
    core->cnt = 0;
  } else {
    core->cnt--;
  }
  spinrwlockunlock(core);
}


/**
 * Promote a reader lock to the writer lock.
 */
bool SpinRWLock::promote() {
  assert(true);
  SpinRWLockCore* core = (SpinRWLockCore*)opq_;
  spinrwlocklock(core);
  if (core->cnt > 1) {
    spinrwlockunlock(core);
    return false;
  }
  core->cnt = INT32MAX;
  spinrwlockunlock(core);
  return true;
}


/**
 * Demote the writer lock to a reader lock.
 */
void SpinRWLock::demote() {
  assert(true);
  SpinRWLockCore* core = (SpinRWLockCore*)opq_;
  spinrwlocklock(core);
  core->cnt = 1;
  spinrwlockunlock(core);
}


/**
 * Lock the semephore of SpinRWLock.
 */
static void spinrwlocklock(SpinRWLockCore* core) {
   assert(core);
  if (::pthread_spin_lock(&core->sem) != 0) throw std::runtime_error("pthread_spin_lock");
 }


/**
 * Unlock the semephore of SpinRWLock.
 */
static void spinrwlockunlock(SpinRWLockCore* core) {
#if defined(_SYS_MSVC_) || defined(_SYS_MINGW_)
  assert(core);
  ::InterlockedExchange(&core->sem, 0);
#elif _KC_GCCATOMIC
  assert(core);
  __sync_lock_release(&core->sem);
#else
  assert(core);
  if (::pthread_spin_unlock(&core->sem) != 0) throw std::runtime_error("pthread_spin_unlock");
#endif
}


/**
 * SlottedRWLock internal.
 */
struct SlottedSpinRWLockCore {
   ::pthread_spinlock_t sems[LOCKSEMNUM]; ///< semaphores
   uint32_t* cnts;                        ///< counts of threads
  size_t slotnum;                        ///< number of slots
};


/**
 * Lock the semephore of SlottedSpinRWLock.
 * @param core the internal fields.
 * @param idx the index of the semaphoe.
 */
static void slottedspinrwlocklock(SlottedSpinRWLockCore* core, size_t idx);


/**
 * Unlock the semephore of SlottedSpinRWLock.
 * @param core the internal fields.
 * @param idx the index of the semaphoe.
 */
static void slottedspinrwlockunlock(SlottedSpinRWLockCore* core, size_t idx);


/**
 * Constructor.
 */
SlottedSpinRWLock::SlottedSpinRWLock(size_t slotnum) : opq_(NULL) { 
  assert(true);
  SlottedSpinRWLockCore* core = new SlottedSpinRWLockCore;
  ::pthread_spinlock_t* sems = core->sems;
  uint32_t* cnts = new uint32_t[slotnum];
  for (size_t i = 0; i < LOCKSEMNUM; i++) {
    if (::pthread_spin_init(sems + i, PTHREAD_PROCESS_PRIVATE) != 0)
      throw std::runtime_error("pthread_spin_init");
  }
  for (size_t i = 0; i < slotnum; i++) {
    cnts[i] = 0;
  }
  core->cnts = cnts;
  core->slotnum = slotnum;
  opq_ = (void*)core;
 
}


/**
 * Destructor.
 */
SlottedSpinRWLock::~SlottedSpinRWLock() {
 
  assert(true);
  SlottedSpinRWLockCore* core = (SlottedSpinRWLockCore*)opq_;
  ::pthread_spinlock_t* sems = core->sems;
  for (size_t i = 0; i < LOCKSEMNUM; i++) {
    ::pthread_spin_destroy(sems + i);
  }
  delete[] core->cnts;
  delete core; 
}


/**
 * Get the writer lock of a slot.
 */
void SlottedSpinRWLock::lock_writer(size_t idx) {
  assert(true);
  SlottedSpinRWLockCore* core = (SlottedSpinRWLockCore*)opq_;
  size_t semidx = idx % LOCKSEMNUM;
  slottedspinrwlocklock(core, semidx);
  uint32_t wcnt = 0;
  while (core->cnts[idx] > 0) {
    slottedspinrwlockunlock(core, semidx);
    if (wcnt >= LOCKBUSYLOOP) {
      Thread::chill();
    } else {
      Thread::yield();
      wcnt++;
    }
    slottedspinrwlocklock(core, semidx);
  }
  core->cnts[idx] = INT32MAX;
  slottedspinrwlockunlock(core, semidx);
}


/**
 * Get the reader lock of a slot.
 */
void SlottedSpinRWLock::lock_reader(size_t idx) {
  assert(true);
  SlottedSpinRWLockCore* core = (SlottedSpinRWLockCore*)opq_;
  size_t semidx = idx % LOCKSEMNUM;
  slottedspinrwlocklock(core, semidx);
  uint32_t wcnt = 0;
  while (core->cnts[idx] >= (uint32_t)INT32MAX) {
    slottedspinrwlockunlock(core, semidx);
    if (wcnt >= LOCKBUSYLOOP) {
      Thread::chill();
    } else {
      Thread::yield();
      wcnt++;
    }
    slottedspinrwlocklock(core, semidx);
  }
  core->cnts[idx]++;
  slottedspinrwlockunlock(core, semidx);
}


/**
 * Release the lock of a slot.
 */
void SlottedSpinRWLock::unlock(size_t idx) {
  assert(true);
  SlottedSpinRWLockCore* core = (SlottedSpinRWLockCore*)opq_;
  size_t semidx = idx % LOCKSEMNUM;
  slottedspinrwlocklock(core, semidx);
  if (core->cnts[idx] >= (uint32_t)INT32MAX) {
    core->cnts[idx] = 0;
  } else {
    core->cnts[idx]--;
  }
  slottedspinrwlockunlock(core, semidx);
}


/**
 * Get the writer locks of all slots.
 */
void SlottedSpinRWLock::lock_writer_all() {
  assert(true);
  SlottedSpinRWLockCore* core = (SlottedSpinRWLockCore*)opq_;
  uint32_t* cnts = core->cnts;
  size_t slotnum = core->slotnum;
  for (size_t i = 0; i < slotnum; i++) {
    size_t semidx = i % LOCKSEMNUM;
    slottedspinrwlocklock(core, semidx);
    uint32_t wcnt = 0;
    while (cnts[i] > 0) {
      slottedspinrwlockunlock(core, semidx);
      if (wcnt >= LOCKBUSYLOOP) {
        Thread::chill();
      } else {
        Thread::yield();
        wcnt++;
      }
      slottedspinrwlocklock(core, semidx);
    }
    cnts[i] = INT32MAX;
    slottedspinrwlockunlock(core, semidx);
  }
}


/**
 * Get the reader locks of all slots.
 */
void SlottedSpinRWLock::lock_reader_all() {
  assert(true);
  SlottedSpinRWLockCore* core = (SlottedSpinRWLockCore*)opq_;
  uint32_t* cnts = core->cnts;
  size_t slotnum = core->slotnum;
  for (size_t i = 0; i < slotnum; i++) {
    size_t semidx = i % LOCKSEMNUM;
    slottedspinrwlocklock(core, semidx);
    uint32_t wcnt = 0;
    while (cnts[i] >= (uint32_t)INT32MAX) {
      slottedspinrwlockunlock(core, semidx);
      if (wcnt >= LOCKBUSYLOOP) {
        Thread::chill();
      } else {
        Thread::yield();
        wcnt++;
      }
      slottedspinrwlocklock(core, semidx);
    }
    cnts[i]++;
    slottedspinrwlockunlock(core, semidx);
  }
}


/**
 * Release the locks of all slots.
 */
void SlottedSpinRWLock::unlock_all() {
  assert(true);
  SlottedSpinRWLockCore* core = (SlottedSpinRWLockCore*)opq_;
  uint32_t* cnts = core->cnts;
  size_t slotnum = core->slotnum;
  for (size_t i = 0; i < slotnum; i++) {
    size_t semidx = i % LOCKSEMNUM;
    slottedspinrwlocklock(core, semidx);
    if (cnts[i] >= (uint32_t)INT32MAX) {
      cnts[i] = 0;
    } else {
      cnts[i]--;
    }
    slottedspinrwlockunlock(core, semidx);
  }
}


/**
 * Lock the semephore of SlottedSpinRWLock.
 */
static void slottedspinrwlocklock(SlottedSpinRWLockCore* core, size_t idx) { 
  assert(core);
  if (::pthread_spin_lock(core->sems + idx) != 0) throw std::runtime_error("pthread_spin_lock");
 
}


/**
 * Unlock the semephore of SlottedSpinRWLock.
 */
static void slottedspinrwlockunlock(SlottedSpinRWLockCore* core, size_t idx) {
 
  assert(core);
  if (::pthread_spin_unlock(core->sems + idx) != 0)
    throw std::runtime_error("pthread_spin_unlock");
 
}


/**
 * Default constructor.
 */
CondVar::CondVar() : opq_(NULL) {
 
  assert(true);
  CondVarCore* core = new CondVarCore;
  if (::pthread_cond_init(&core->cond, NULL) != 0)
    throw std::runtime_error("pthread_cond_init");
  opq_ = (void*)core;
 
}


/**
 * Destructor.
 */
CondVar::~CondVar() {
 
  assert(true);
  CondVarCore* core = (CondVarCore*)opq_;
  ::pthread_cond_destroy(&core->cond);
  delete core;
 
}


/**
 * Wait for the signal.
 */
void CondVar::wait(Mutex* mutex) {
 
  assert(mutex);
  CondVarCore* core = (CondVarCore*)opq_;
  ::pthread_mutex_t* mymutex = (::pthread_mutex_t*)mutex->opq_;
  if (::pthread_cond_wait(&core->cond, mymutex) != 0)
    throw std::runtime_error("pthread_cond_wait");
 
}


/**
 * Wait for the signal.
 */
bool CondVar::wait(Mutex* mutex, double sec) { 
  assert(mutex && sec >= 0);
  if (sec <= 0) return false;
  CondVarCore* core = (CondVarCore*)opq_;
  ::pthread_mutex_t* mymutex = (::pthread_mutex_t*)mutex->opq_;
  struct ::timeval tv;
  struct ::timespec ts;
  if (::gettimeofday(&tv, NULL) == 0) {
    double integ;
    double fract = std::modf(sec, &integ);
    ts.tv_sec = tv.tv_sec + (time_t)integ;
    ts.tv_nsec = (long)(tv.tv_usec * 1000.0 + fract * 999999000);
    if (ts.tv_nsec >= 1000000000) {
      ts.tv_nsec -= 1000000000;
      ts.tv_sec++;
    }
  } else {
    ts.tv_sec = std::time(NULL) + 1;
    ts.tv_nsec = 0;
  }
  int32_t ecode = ::pthread_cond_timedwait(&core->cond, mymutex, &ts);
  if (ecode == 0) return true;
  if (ecode != ETIMEDOUT && ecode != EINTR)
    throw std::runtime_error("pthread_cond_timedwait");
  return false;
 
}


/**
 * Send the wake-up signal to another waiting thread.
 */
void CondVar::signal() {
 
  assert(true);
  CondVarCore* core = (CondVarCore*)opq_;
  if (::pthread_cond_signal(&core->cond) != 0)
    throw std::runtime_error("pthread_cond_signal");
 
}


/**
 * Send the wake-up signals to all waiting threads.
 */
void CondVar::broadcast() {
 
  assert(true);
  CondVarCore* core = (CondVarCore*)opq_;
  if (::pthread_cond_broadcast(&core->cond) != 0)
    throw std::runtime_error("pthread_cond_broadcast");
 
}


/**
 * Default constructor.
 */
TSDKey::TSDKey() : opq_(NULL) {
   assert(true);
  ::pthread_key_t* key = new ::pthread_key_t;
  if (::pthread_key_create(key, NULL) != 0)
    throw std::runtime_error("pthread_key_create");
  opq_ = (void*)key;
 }


/**
 * Constructor with the specifications.
 */
TSDKey::TSDKey(void (*dstr)(void*)) : opq_(NULL) {
 
  assert(true);
  ::pthread_key_t* key = new ::pthread_key_t;
  if (::pthread_key_create(key, dstr) != 0)
    throw std::runtime_error("pthread_key_create");
  opq_ = (void*)key;
 
}


/**
 * Destructor.
 */
TSDKey::~TSDKey() {
 
  assert(true);
  ::pthread_key_t* key = (::pthread_key_t*)opq_;
  ::pthread_key_delete(*key);
  delete key;
 
}


/**
 * Set the value.
 */
void TSDKey::set(void* ptr) {
 
  assert(true);
  ::pthread_key_t* key = (::pthread_key_t*)opq_;
  if (::pthread_setspecific(*key, ptr) != 0)
    throw std::runtime_error("pthread_setspecific");
 
}


/**
 * Get the value.
 */
void* TSDKey::get() const {
 
  assert(true);
  ::pthread_key_t* key = (::pthread_key_t*)opq_;
  return ::pthread_getspecific(*key);
 
}


/**
 * Set the new value.
 */
int64_t AtomicInt64::set(int64_t val) {
 
  assert(true);
  lock_.lock();
  int64_t oval = value_;
  value_ = val;
  lock_.unlock();
  return oval;
 
}


/**
 * Add a value.
 */
int64_t AtomicInt64::add(int64_t val) {
 
  assert(true);
  lock_.lock();
  int64_t oval = value_;
  value_ += val;
  lock_.unlock();
  return oval;
 
}


/**
 * Perform compare-and-swap.
 */
bool AtomicInt64::cas(int64_t oval, int64_t nval) {
  assert(true);
  bool rv = false;
  lock_.lock();
  if (value_ == oval) {
    value_ = nval;
    rv = true;
  }
  lock_.unlock();
  return rv;
}


/**
 * Get the current value.
 */
int64_t AtomicInt64::get() const {
  assert(true);
  lock_.lock();
  int64_t oval = value_;
  lock_.unlock();
  return oval;
}


}                                        
}
