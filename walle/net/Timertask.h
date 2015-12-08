#ifndef WALLE_TIMERTASK_H_
#define WALLE_TIMERTASK_H_
#include <walle/sys/wallesys.h>
#include <walle/net/Callback.h>
#include <stdint.h>
using namespace walle::sys;
namespace walle {
namespace net {
class TimerTask {
 public:
  TimerTask(const TimerCallback& cb, const Time& when,
                const int64_t& interval)
    : _callback(cb),
      _expiration(when),
      _interval(interval),
      _repeat(interval > 0),
      _sequence(_gTimerNum.incrementAndGet())
  { }

 void run() const
  {
    _callback();
  }

  Time& expiration()  { return _expiration; }
  bool repeat() const { return _repeat; }
  int64_t sequence() const { return _sequence; }

  void restart(const Time &now);

  int64_t step() const { return _interval; }
  
  static int64_t numCreated() { return _gTimerNum.get(); }

 private:
  const TimerCallback _callback;
  Time _expiration;
  const int64_t _interval;
  const bool _repeat;
  const int64_t _sequence;

  static AtomicInt64 _gTimerNum;
};
}
}

#endif
