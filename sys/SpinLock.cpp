#include <walle/sys/wallesys.h>


namespace walle {
namespace sys {
	SpinLock::SpinLock()
	{

	  ::pthread_spin_init(&_spin, PTHREAD_PROCESS_PRIVATE) ;
	
	}
	
	SpinLock::~SpinLock()
	{
	  ::pthread_spin_destroy(&_spin);
	}
	
	void SpinLock::lock()
	{
	  ::pthread_spin_lock(&_spin);
	}
	
	bool SpinLock::lock_try() {
	  int32_t ecode = ::pthread_spin_trylock(&_spin);
	  if (ecode == 0) return true;
		return false;
	}
	
	void SpinLock::unlock()
	{
	  ::pthread_spin_unlock(&_spin);
	}

}
}
