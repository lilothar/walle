#ifndef WALLE_EVENT_TIMEWHEEL_H_
#define WALLE_EVENT_TIMEWHEEL_H_
#include <walle/algo/noncopyable.h>
#include <walle/algo/functional.h>
#include <walle/algo/memory.h>
#include <vector>
#include <stddef.h>
#include <stdint.h>
#include <walle/net/Timertask.h>
#include <walle/net/TimerId.h>
namespace walle {
namespace net {

class EventLoop;

struct  TimerWheelHander{
    std::shared_ptr<TimerTask> ptr; 
    void release()
    {
        if(ptr) {
            ptr.reset();
        }
    }

    bool valid()
    {
        return ptr ? true : false;
    }
   
};

class TimerWheel :public std::noncopyable {
public:
    static const int64_t kDefaultNum = 60;
    typedef std::vector<std::weak_ptr<TimerTask> > TimerWheelList;
public:
    /*
        numb must be > 0;
        */
    TimerWheel(EventLoop *loop, int64_t numb = kDefaultNum);

    ~TimerWheel();
    
    TimerWheelHander addTimer(const TimerCallback&factor,
                                    int64_t delaySec,
                                    bool repeat
                                    );

    void start();
    void stop();
private:
    void onTimer();
    void addTimerInLoop(const std::shared_ptr<TimerTask> &task);
    void stopInLoop(); 
private:
    size_t _numBuckets;
    size_t _index;
    std::vector<TimerWheelList > _buffer;
    EventLoop   *_loop;
    TimerId      _timerid;
    bool         _stoped;
    
};

}
}
#endif