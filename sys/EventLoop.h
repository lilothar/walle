#ifndef WALLE_EVENTLOOP_H_
#define WALLE_EVENTLOOP_H_
#include <pthread.h>
#include <walle/sys/TaskQueue.h>
#include <walle/sys/Time.h>
#include <walle/sys/Poller.h>
#include <walle/sys/Mutex.h>

namespace walle {
namespace sys {
class Poller;
class EventLoop{
    public:
        EventLoop();
        ~EventLoop();

        void updateIoc(IOComponet *ioc, bool writeon, bool readon);
        void addIoc(IOComponet *ioc, bool writeon, bool readon);
        void removeIoc(IOComponet *ioc);

        void loop();
        bool assertInloop();
        void wakeUp();
        void runInloop(Task *task, bool ignorePriority =false);
        bool isInLoop();
		void quit();
        
       static EventLoop *getThreadLoop();
   private:
        void doIocTaskInLoop(const IOEvent&ioe);
        void dodelayOptTask();
        void doTaskInloop(Task *t);
        void doPriorityTask();
        void processIoEvent(IOEvent &ev);
   private:
        bool             _runnig;
        bool             _quit;
        const pthread_t  _thid;
        Poller          *_poller;
        TaskQueue        _taskQueue;
        Time             _lastPollTime;
        map<int64_t, IOComponet*> _iocMap;
        IOEventList               _activeIoc;
        IOEventList               _delayOptTask;
        Mutex                     _mutex;
        IOComponet                *_wakeUpIoc;
    
};
}
}
#endif
