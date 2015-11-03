#ifndef WALLE_NET_BLOCKQUEUE_H_
#define WALLE_NET_BLOCKQUEUE_H_

#include <walle/sys/Cond.h>
#include <walle/sys/Mutex.h>
#include <walle/sys/ScopeLock.h>
#include <walle/algo/noncopyable.h>
#include <deque>
#include <assert.h>

namespace walle {
namespace sys {

template<typename T>
    
class BlockQueue : std::noncopyable
{
 public:
  BlockQueue()
    : _mutex(),
      _notEmpty(),
      _queue()
  {
  }

  void put(const T& x)
  {
    ScopeMutex lock(&_mutex);
    _queue.push_back(x);
    _notEmpty.signal();
  }

  T take()
  {
    ScopeMutex lock(&_mutex);
    while (_queue.empty())
    {
      _notEmpty.wait(&_mutex);
    }
    assert(!_queue.empty());
    T front(_queue.front());
    _queue.pop_front();
    return front;
  }

  size_t size() const
  {
    ScopeMutex lock(&_mutex);
    return _queue.size();
  }

 private:
  Mutex             _mutex;
  Cond             _notEmpty;
  std::deque<T>     _queue;
};

}
}

#endif
