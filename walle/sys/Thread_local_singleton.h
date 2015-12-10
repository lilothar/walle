#ifndef WALLE_THREAD_THREADLOCALSINGLETON_H_
#define WALLE_THREAD_THREADLOCALSINGLETON_H_

#include <walle/algo/noncopyable.h>
#include <assert.h>
#include <pthread.h>

namespace walle {
namespace sys {

template<typename T>
class ThreadLocalSingleton : std::noncopyable
{
 public:

  static T& instance()
  {
    if (!_t_value)
    {
      _t_value = new T();
      _deleter.set(_t_value);
    }
    return *_t_value;
  }

  static T* pointer()
  {
    return _t_value;
  }

 private:
  ThreadLocalSingleton();
  ~ThreadLocalSingleton();

  static void destructor(void* obj)
  {
    assert(obj == _t_value);
    typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
    T_must_be_complete_type dummy; (void) dummy;
    delete _t_value;
    _t_value = 0;
  }

  class Deleter
  {
   public:
    Deleter()
    {
      pthread_key_create(&_pkey, &ThreadLocalSingleton::destructor);
    }

    ~Deleter()
    {
      pthread_key_delete(_pkey);
    }

    void set(T* newObj)
    {
      assert(pthread_getspecific(_pkey) == NULL);
      pthread_setspecific(_pkey, newObj);
    }

    pthread_key_t _pkey;
  };

  static __thread T* _t_value;
  static Deleter _deleter;
};

template<typename T>
__thread T* ThreadLocalSingleton<T>::_t_value = 0;

template<typename T>
typename ThreadLocalSingleton<T>::Deleter ThreadLocalSingleton<T>::_deleter;

}
}
#endif
