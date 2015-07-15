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
       void wakeUp();
       void runTask(ITask *task);
       void loop();
       bool addIoc(IOComponent*ioc, bool readon, bool writeon);
       bool updateIoc(IOComponent*ioc, bool readon, bool writeon);
       void removeIoc(IOComponent*ioc);
    private:
       void queueTask(ITask *task);
       void queueTimerTask(TimerTask *task);
       void runTaskInloop(ITask *task);
       void runTimerTaskInloop(TimerTask *task);
    private:
		Poller               *_poller;
		Timer                *_Timer;
        TaskQueue<ITask* >    _taskQueue;
        TaskQueue<TimerTask*> _timerTaskqueue;
        
        
};
}
}
#endif
