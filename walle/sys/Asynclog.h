#ifndef WALLE_SYS_ASYNCLOG_H_
#define WALLE_SYS_ASYNCLOG_H_
#include <walle/sys/Blockqueue.h>
#include <walle/sys/Fixbuffer.h>
#include <walle/sys/CountDown.h>
#include <walle/sys/Mutex.h>
#include <walle/sys/Thread.h>
#include <walle/algo/functional.h>
#include <walle/algo/noncopyable.h>
#include <vector>

namespace walle {
namespace sys{

class LogFile;
class AsyncLogging :std::noncopyable
{
 public:

  AsyncLogging(const string& basename,
               size_t rollSize,
               int flushInterval = 3);

  ~AsyncLogging()
  {
    if (_isrunning)
    {
      stop();
    }
  }

  void append(const char* logline, size_t len);
  void waitStart() 
  {
    if (_isrunning)
        return;
    _isrunning = true;
    _th.start();
    _latch.wait();
  }
  void stop()
  {
    if (_isrunning) {
        _isrunning = false;
        _cond.signal();
        _th.join();
    }
  }
  void run();


 private:
     void flushall(LogFile* output);

  // declare but not define, prevent compiler-synthesized functions
  AsyncLogging(const AsyncLogging&);  // ptr_container
  void operator=(const AsyncLogging&);  // ptr_container

  typedef FixBuffer<klargebuffer> Buffer;
  typedef vector<Buffer*> BufferVector;
 
  const int     _flushInterval;
  bool          _isrunning;
  string        _basename;
  size_t        _rollSize;
  CountDown     _latch;
  Mutex         _mutex;
  Cond          _cond;
  Buffer       *_currentBuffer;
  Buffer       *_nextBuffer;
  BufferVector _buffers;
  Thread       _th;
};

}

}
#endif

