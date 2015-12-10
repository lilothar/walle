#include <walle/sys/Thread.h>
#include <walle/sys/LocalThread.h>
#include <walle/sys/Logging.h>
#include <walle/algo/memory.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

namespace walle {
namespace sys {

namespace detail {

struct ThreadData
{
  typedef walle::sys::Thread::ThreadFunc ThreadFunc;
  ThreadFunc _func;
  string _name;
  std::weak_ptr<pid_t> _wkTid;

  ThreadData(const ThreadFunc& func,
             const string& name,
             const std::shared_ptr<pid_t>& tid)
    : _func(func),
      _name(name),
      _wkTid(tid)
  { }

  void runInThread()
  {
    pid_t tid = walle::LocalThread::tid();

    std::shared_ptr<pid_t> ptid = _wkTid.lock();
    if (ptid)
    {
      *ptid = tid;
      ptid.reset();
    }

    walle::LocalThread::setThreadName(_name.empty() ? "walleThread" : _name.c_str());
    ::prctl(PR_SET_NAME, walle::LocalThread::t_threadName);
      _func();
      walle::LocalThread::setThreadName("finished");
    
  }
};

void* startThread(void* obj)
{
  ThreadData* data = static_cast<ThreadData*>(obj);
  data->runInThread();
  delete data;
  return NULL;
}


}
AtomicInt32 Thread::_numCreated;

Thread::Thread(const ThreadFunc& func, const string& n)
  : _started(false),
    _joined(false),
    _pthreadId(0),
    _tid(new pid_t(0)),
    _func(func),
    _name(n),
    _seq(_numCreated.incrementAndGet())
{
  setDefaultName();
}

Thread::~Thread()
{
  if (_started && !_joined)
  {
    pthread_detach(_pthreadId);
  }
}

void Thread::setDefaultName()
{
  int num = _numCreated.get();
  if (_name.empty())
  {
    char buf[32];
    snprintf(buf, sizeof buf, "Thread%d", num);
    _name = buf;
  }
}

void Thread::start()
{
  assert(!_started);
  _started = true;
  // FIXME: move(_func)
  detail::ThreadData* data = new detail::ThreadData(_func, _name, _tid);
  if (pthread_create(&_pthreadId, NULL, &detail::startThread, data))
  {
    _started = false;
    delete data; // or no delete?
    LOG_ERROR << "Failed in pthread_create";
  }
}

int Thread::join()
{
  assert(_started);
  assert(!_joined);
  _joined = true;
  return pthread_join(_pthreadId, NULL);
}
bool Thread::detach()
{  
	if (!_started) 
		return true;
	if (::pthread_detach(_pthreadId) != 0)
			return false;  
	return true;
}
/** * Terminate the running thread. */
void Thread::exit()
{
	::pthread_exit(NULL);
}

/** * Yield the processor from the current thread. */

void Thread::yield()
{
	::sched_yield();
}

/** * Suspend execution of the current thread. */

bool Thread::sleep(int64_t usec)
{
	struct timespec ts = { 0, 0 };
	ts.tv_sec = static_cast<time_t>(usec / Time::kMicroSecondsPerSecond);
	ts.tv_nsec = static_cast<long>(usec % Time::kMicroSecondsPerSecond * 1000);
	::nanosleep(&ts, NULL);
	return true;

}

int    Thread::seq() {return _seq; }
/** * Get the hash value of the current thread. */
int64_t Thread::hash()
{
	pthread_t tid = LocalThread::tid();
	int64_t num;
	if (sizeof(tid) == sizeof(num)) {
		::memcpy(&num, &tid, sizeof(num));
	} else if (sizeof(tid) == sizeof(int32_t)) {
		uint32_t inum;
		::memcpy(&inum, &tid, sizeof(inum));
		num = inum;
	} else {
		num = StringUtil::hashmurmur(&tid, sizeof(tid));
	}
	
	return num;
}
}
}
