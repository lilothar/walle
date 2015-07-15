#ifndef WALLE_EVENTLOOP_H_
#define WALLE_EVNETLOOP_H_

#include <pthread.h>

namespace walle{
namespace net{

class Timer;
class TimerTask;
class EventLoop{
    public:
       EventLoop();
       ~EventLoop();
	   bool isInLoopThread();
	   void addTimer(TimerTask *task);
       void wakeUp();
       void runTask(ITask *task);
       void loop();
	 private:
	 	friend class IOComponent;
	   void inQueueIoc(IOComponent *ioc);
       bool addIoc(IOComponent*ioc, bool readon, bool writeon);
       bool updateIoc(IOComponent*ioc, bool readon, bool writeon);
       void removeIoc(IOComponent*ioc);
   	   void doDelayedIocOpt();
	   void dotaskInloop();
       void runTimerTaskInloop();
    private:
		Poller                *_poller;
		Timer                 *_timer;
        TaskQueue<ITask* >     _taskQueue;
		vector<ITask* >         _mytask;
        TaskQueue<TimerTask*>   _timerTaskqueue;
		vector<TimerTask*>      _mytimer;
		TaskQueue<IOComponent*> _delayedIoc;
		vector<IOComponent*>    _myIoc;
		pthread_t               _thid;
		bool                    _quit;
        
        
};
}
}
#endif
