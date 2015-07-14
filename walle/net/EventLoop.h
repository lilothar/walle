#ifndef WALLE_EVENTLOOP_H_
#define WALLE_EVNETLOOP_H_


namespace walle{
namespace net{

class Timer;
class TimerTask;
class EventLoop{
    public:
       EventLoop();
       ~EventLoop();
	   bool isInLoopThread();
	   void runAt(TimerTask *task);
    private:
		Poller  *_poller;
		Timer   *_Timer;
        
};
}
}
#endif
