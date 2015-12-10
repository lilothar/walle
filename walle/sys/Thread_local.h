#ifndef WALLE_THREAD_THREADLOCAL_H_
#define WALLE_THREAD_THREADLOCAL_H_

#include <walle/algo/noncopyable.hpp>
#include <pthread.h>

namespace walle {
namespace sys {

template<typename T>
class ThreadLocal : boost::noncopyable
{
 public:
  ThreadLocal()
  {
    pthread_key_create(&_pkey, &ThreadLocal::destructor);
  }

  ~ThreadLocal()
  {
    pthread_key_delete(_pkey);
  }

  T& value()
  {
    T* perThreadValue = static_cast<T*>(pthread_getspecific(_pkey));
    if (!perThreadValue)
    {
      T* newObj = new T();
      pthread_setspecific(_pkey, newObj);
      perThreadValue = newObj;
    }
    return *perThreadValue;
  }

 private:

  static void destructor(void *x)
  {
    T* obj = static_cast<T*>(x);
    typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
    T_must_be_complete_type dummy; (void) dummy;
    delete obj;
  }

 private:
  pthread_key_t _pkey;
};

}
}
#endif
