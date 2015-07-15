#ifndef WALLE_ITASK_QUEUE_H_
#define WALLE_ITASK_QUEUE_H_
#include <walle/sys/Mutex.h>
#include <walle/sys/ScopeLock.h>
#include <list>
#include <walle/net/ITask.h>

using walle::sys::Mutex;
using walle::sys::ScopeMutex;
namespace walle {
namespace net {

template <typename T>
    
class TaskQueue{
    public:
        TaskQueue():_mutex(),_queue() {}
        ~TaskQueue(){}
        size_t size()
        {
            return _queue.size();
        }

        void push(T task)
        {
            ScopeMutex lock(&_mutex);
            _queue.push_back(task);
        }

        T pop()
        {
             ScopeMutex lock(&_mutex);
             if(_queue.empty()) {
                return NULL;
             }
             T t(_queue.front());
             _queue.pop_front();
             return t;
        }
        void swap(ITaskQueue &rhs)
        {
            ScopeMutex lock(&_mutex);
            _queue.swap(rhs._queue)
        }

        bool empty()
        {
            return _queue.empty();
        }
          
    private:
        Mutex             _mutex;
        std::list<T> _queue;
};
}
}
#endif
