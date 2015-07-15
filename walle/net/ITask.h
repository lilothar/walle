#ifndef WALLE_ITASK_H_
#define WALLE_ITASK_H_
#include <walle/sys/Time.h>
 
using walle::sys::Time;

namespace walle{

namespace net{

class ITask{
    public:
        ITask():_userData(0),_epochTime(Time::now()){}
        virtual ~ITask();
        virtual void runTask() = 0;
        void setUserData(void *data) { _userData = data; }
        void*getUserData() { return _userData; }
    protected:
        void  *_userData;
        Time   _epochTime;
};
}
}
#endif
