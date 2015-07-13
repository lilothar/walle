#include <walle/sys/wallesys.h>


namespace walle{
namespace sys {

Cond::Cond()
{
	  ::pthread_cond_init(&_cond, NULL);
}
	
Cond::~Cond()
{
	  ::pthread_cond_destroy(&_cond);
}
	
void Cond::wait(Mutex* mutex)
{
	  assert(mutex);
	  ::pthread_cond_wait(&_cond, &mutex->_mutex);
}
	
bool Cond::wait(Mutex* mutex, int32_t usec)
{
	  assert(mutex && usec >= 0);
	  if (usec <= 0) return false;
	  struct ::timeval tv;
	  struct ::timespec ts;
	  if (::gettimeofday(&tv, NULL) == 0) {
		int32_t integ = usec/1000000;
		int32_t fract = usec%1000000;
		ts.tv_sec = tv.tv_sec + integ;
		ts.tv_nsec = tv.tv_usec * 1000 + fract*1000;
		if (ts.tv_nsec >= 1000000000) {
		  ts.tv_nsec -= 1000000000;
		  ts.tv_sec++;
		}
	  } else {
		ts.tv_sec = ::time(NULL) + 1;
		ts.tv_nsec = 0;
	  }
	  int32_t ecode = ::pthread_cond_timedwait(&_cond, &mutex->_mutex, &ts);
	  if (ecode == 0) return true;
	  return false;
	}
	
void Cond::signal() 
{
	  assert(true);
	  ::pthread_cond_signal(&_cond);
}
	
void Cond::broadcast()
{
	  assert(true);
	  ::pthread_cond_broadcast(&_cond);
}
}
}
