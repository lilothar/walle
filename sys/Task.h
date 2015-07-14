#ifndef WALLE_TASK_H_
#define WALLE_TASK_H_

#include <walle/sys/Atomic.h>
#include <stdint.h>

namespace walle {
namespace sys {

class Task{
    public:
       typedef void (*Taskfun)(void *args,Task *task);
	public:
	  Task(Taskfun factor, void *args=NULL,int32_t priority = 100);
	   ~Task(){}
	   void runTask();
	   int64_t taskSeq() { return _seq; }
	   void    cancel() { _canceled = true;}
	   bool    isCancel() { return _canceled; }
       int32_t priority() {return _priority;}

    protected:
        void        *_args;
		int64_t      _seq;
        int32_t      _priority;
		bool         _canceled;
		Taskfun      _factor;
		static AtomicInt64 gSeqnum;
	};
struct taskcmp{
    bool operator()( Task *t1,  Task *t2) 
    {
        return t1->priority() < t2->priority();
    }
};

}
}
#endif
