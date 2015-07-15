#ifndef WALLE_TIME_H_
#define WALLE_TIME_H_
#include <walle/net/TimerTask.h>

namespace walle{
namespace net{
class EventLoop;
class EventFD;

class Timer{
    public:
        Timer(EventLoop* loop);
        ~Timer();
        
        bool init();

        bool  add(const TimerTask *timerTask);

        void  cancel(const TimerTask *timerTask);
      
   private:
        typedef std::pair<Time, TimerTask*> TaskEntry;
        typedef std::set<TaskEntry >        TaskSet;

        EventLoop         *_owner;
        EventFD           *_timerfd;
        bool               _calling;
        
    

};
}
}
#endif
