#ifndef WALLE_TIMERTASK_H_
#define WALLE_TIMERTASK_H_
#include <walle/sys/Time.h>
#include <walle/sys/Atomic.h>
#include <stdint.h>

using walle::sys::AtomicInt64;
using walle::sys::Time;
namespace walle{
namespace net{

class TimerTask{
    public:
       TimerTask(const Time& expire, int64_t inter = 0, void *user_data = NULL);
       virtual  ~TimerTask();
       virtual void run() = 0;
       Time expiration() { return _expiration; }
       bool repeat() { return _repeat; }
       int64_t sequence() { return _sequence; }
       int64_t interval() { return _inerval; }

       void reset(const Time & t);
       //for repeat timer task;
       void restart();
       bool isExpired() { return _expiration <= Time::now(); }
       static int64_t creatednum() { return gSeqnum.get(); }
    private:
        void               *_userData;
        Time                _expiration;
        const int64_t       _inerval;
        const bool          _repeat;
        const int64_t       _sequence;
        static AtomicInt64  gSeqnum;
};
}
}
#endif
