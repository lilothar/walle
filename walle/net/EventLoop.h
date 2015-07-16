#ifndef WALLE_EVENTLOOP_H_
#define WALLE_EVNETLOOP_H_

#include <pthread.h>
#include <walle/sys/wallesys.h>
#include <walle/net/ITask.h>
#include <walle/net/TaskQueue.h>


namespace walle{
namespace net{

class EventLoop{
    public:
       EventLoop();
       ~EventLoop();
	   bool isInLoopThread();
       void runTask(ITask *task);
       void loop();
	   void assertInloop();
	   static EventLoop* getLocalEventLoop();
	private:
	   void doTaskInloop();
    private:
		//Poller                *_poller;
        TaskQueue<ITask* >     _taskQueue;
		vector<ITask* >         _mytask;
		bool                    _quit;
		pthread_t               _thid;
        
        
};
}
}
#endif
