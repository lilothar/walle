#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <cstring>
#include <cmath>
#include <stdio.h>
#include <walle/sys/wallesys.h>


/**
 * Thread internal.
 */
namespace walle{
	namespace LocalThread
	{
	  __thread pthread_t t_tid = 0;
	  __thread char t_tidString[32];
	  __thread size_t t_tidStringLength= 0;
      __thread char t_threadName[32]="";
	
    void setThreadName(const char* str)
    {
        if(!str) {
          snprintf(t_threadName,15, "%s", "unknown");  
        }
             
        snprintf(t_threadName,15, "%s", str);
    }
	void cacheTid()
	{
	  if (t_tid == 0) {
	    	t_tid = ::pthread_self();
	   		 t_tidStringLength = snprintf(t_tidString, sizeof(t_tidString), "%lu", 
			t_tid);
            t_tidStringLength = strlen(t_tidString); 
            setThreadName("unknown");
	  }
	}
	bool isMainThread()
	{
 		  return tid() == (pthread_t)::getpid();
	}
}
namespace sys {	
	AtomicInt32 Thread::_numCreated;

/**
 * Call the running thread.
 * @param arg the thread.
 * @return always NULL.
 */
static void* threadrun(void* arg);

/**
 * Default constructor.
 */
Thread::Thread(){
  _seq = _numCreated.incrementAndGet();
  _alive = false;
}


/**
 * Destructor.
 */
Thread::~Thread() {
  if (_alive) {
  		join();
		_alive = false;
  }
  
}


/**
 * Start the thread.
 */
bool Thread::start() {
  if (_alive){
	  return true;
  }
  if (::pthread_create(&_th, NULL, threadrun, this) != 0)
  {
    return false;
  }
  _alive = true;
  return true;
}


/**
 * Wait for the thread to finish.
 */
bool Thread::join() {
  if (!_alive) return true;
  _alive = false;
  if (::pthread_join(_th, NULL) != 0)
	  return false;
  return true;
}


/**
 * Put the thread in the detached state.
 */
bool Thread::detach() {
  if (!_alive) return true;
  if (::pthread_detach(_th) != 0)
	  return false;
  return true;
}


/**
 * Terminate the running thread.
 */
void Thread::exit() {
  ::pthread_exit(NULL);
}


/**
 * Yield the processor from the current thread.
 */
void Thread::yield() {
  ::sched_yield();
}


/**
 * Suspend execution of the current thread.
 */
bool Thread::sleep(int64_t usec)
{
	 struct timespec ts = { 0, 0 };
 	 ts.tv_sec = static_cast<time_t>(usec / Time::kMicroSecondsPerSecond);
 	 ts.tv_nsec = static_cast<long>(usec % Time::kMicroSecondsPerSecond * 1000);
  	 ::nanosleep(&ts, NULL);
     return true;
}
  int    Thread::seq() {return _seq; }

/**
 * Get the hash value of the current thread.
 */
int64_t Thread::hash() {
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


/**
 * Call the running thread.
 */
static void* threadrun(void* arg) {
  Thread* thread = (Thread*)arg;
  LocalThread::tid();
  LOG_INFO<<LocalThread::tid();
  thread->run();
  return NULL;
}
}
}
