#ifndef WALLE_LOCALTHREAD_H_
#define WALLE_LOCALTHREAD_H_
#include <pthread.h>
namespace walle {
namespace LocalThread{

extern __thread pthread_t t_tid;
extern __thread char      t_tidString[32];
extern __thread size_t    t_tidStringLength;
extern __thread char      t_threadName[32];

void cacheTid();

inline pthread_t tid()
{
    if (__builtin_expect(t_tid == 0, 0))
    {
      cacheTid();
    }
    return t_tid;
}

inline const char* tidString() // for logging
{
  return t_tidString;
}

inline size_t tidStringLength() // for logging
{
  return t_tidStringLength;
}

void setThreadName(const char* str);

inline const char* name()
{
  return t_threadName;
}

bool isMainThread();

}

}
#endif
