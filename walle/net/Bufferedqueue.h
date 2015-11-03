#ifndef WALLE_NET_BUFFEREDQUEUE_H_
#define WALLE_NET_BUFFEREDQUEUE_H_

#include <walle/sys/Mutex.h>
#include <walle/sys/ScopeLock.h>
#include <walle/algo/noncopyable.h>
#include <walle/net/Buffer.h>
#include <deque>
#include <assert.h>
#include <walle/config/config.h>

using walle::sys::Mutex;
using walle::sys::ScopeMutex;

namespace walle {
namespace net {
    
class BufferedQueue : std::noncopyable
{
 public:
  BufferedQueue()
    : _bufferedSize(0),
      _mutex(),
      _queue()
      
  {
  }

  void put(Buffer* x)
  {
    ScopeMutex lock(&_mutex);
    _queue.push_back(x);
    _bufferedSize += x->readableBytes();
  }

  Buffer* take()
  {
    ScopeMutex lock(&_mutex);
    if (_queue.empty()) {
        return nullptr;      
    }
    assert(!_queue.empty());
    Buffer  *front = _queue.front();
    _queue.pop_front();
    _bufferedSize -= front->readableBytes();
    return front;
  }

   //num of Buffers
  size_t size() 
  {
    ScopeMutex lock(&_mutex);
    return _queue.size();
  }

  size_t cacheSize()  
  {
    ScopeMutex lock(&_mutex);
    return _bufferedSize;
  }

 private:
  size_t                  _bufferedSize;
  Mutex                   _mutex;
  std::deque<Buffer*>     _queue;
};

}
}

#endif
