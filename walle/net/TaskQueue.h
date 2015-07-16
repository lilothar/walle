#ifndef WALLE_ITASK_QUEUE_H_#define WALLE_ITASK_QUEUE_H_
#include <walle/sys/Mutex.h>
#include <walle/sys/ScopeLock.h>
#include <vector>
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
        }		 size_t popAll(std::vector<T> &result)		 {			ScopeMutex lock(&_mutex);			result.swap(_queue);			return result.size();		 }
        void swap(TaskQueue &rhs)
        {
            ScopeMutex lock(&_mutex);
            _queue.swap(rhs._queue);
        }

        bool empty()
        {			ScopeMutex lock(&_mutex);
            return _queue.empty();
        }		 void clear()		 {			ScopeMutex lock(&_mutex);			_queue.clear();		 }
          
    private:
        Mutex             _mutex;
        std::vector<T> _queue;
};
}
}
#endif
