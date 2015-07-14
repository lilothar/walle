#include <walle/sys/wallesys.h>

namespace walle {
namespace sys {
	Mutex::Mutex() {
	  	if (::pthread_mutex_init(&_mutex, NULL) != 0) {
	  		assert(false);
		}
	}
	
	Mutex::Mutex(Type type) {
	  ::pthread_mutexattr_t attr;
	  ::pthread_mutexattr_init(&attr) ;
	  switch (type) {
		case FAST: {
		  break;
		}
		case ERRORCHECK: {
		  ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
		  break;
		}
		case RECURSIVE: {
			::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		  break;
		}
	  }
	  if (::pthread_mutex_init(&_mutex, &attr) !=0) {

	  	assert(false);
	  }
	}
	
	Mutex::~Mutex() {
	  ::pthread_mutex_destroy(&_mutex);
	}
	
	void Mutex::lock() {
	  	if (::pthread_mutex_lock(&_mutex) != 0) {
	  		assert(false);
		}
	}
	bool Mutex::lock_try() {
	  int32_t ecode = ::pthread_mutex_trylock(&_mutex);
	  if (ecode == 0) return true;
	   return false;
	}
	
	bool Mutex::lock_try(int64_t usec)
	{
		  assert(usec >= 0);
		  struct ::timeval tv;
		  struct ::timespec ts;
		  if (::gettimeofday(&tv, NULL) == 0) {
            
			ts.tv_sec = tv.tv_sec + static_cast<time_t>(usec/1000000);
			ts.tv_nsec =tv.tv_usec * 1000 + static_cast<long>(usec/1000000*1000);
			if (ts.tv_nsec >= 1000000000) {
			  ts.tv_nsec -= 1000000000;
			  ts.tv_sec++;
			}
		  } else {
			ts.tv_sec = ::time(NULL) + 1;
			ts.tv_nsec = 0;
		  }
		  int32_t ecode = ::pthread_mutex_timedlock(&_mutex, &ts);
		  if (ecode == 0) return true;
		  return false;
	}
		
	void Mutex::unlock()
	{
		  assert(true);
		  ::pthread_mutex_unlock(&_mutex);
	}

}

}
		
