#ifndef WALLE_TASKQUEUE_H_
#define WALLE_TASKQUEUE_H_
#include <walle/sys/Task.h>
#include <walle/sys/Mutex.h>
#include <walle/sys/ScopeLock.h>
#include <queue>
#include <vector>
using std::vector;
using std::priority_queue;
namespace walle {
namespace sys {
class TaskQueue{
    public:
        typedef vector<Task*> TaskList;
    public:
        TaskQueue(){}
        ~TaskQueue() 
        { 
           {
            ScopeMutex mutex(&_mutex);
            while(!_queue.empty()) _queue.pop();
           }
        }
        size_t size() { return _queue.size(); }
        void  push(Task *t) 
        {
            ScopeMutex mutex(&_mutex);
            _queue.push(t); 
        }
        Task* pop() 
        {
            ScopeMutex mutex(&_mutex);
            if (_queue.empty()) {
                return NULL;
            }
            Task *r = _queue.top();
            _queue.pop();
            return r;
            
        }
        int multiPop(TaskList &tlist, size_t limit = 0)
        {
            Task *r;
            size_t popsize = limit;
            size_t popedsize = 0;
            ScopeMutex mutex(&_mutex); 
            
            if (limit <=0 || limit > _queue.size()) {
                popsize = _queue.size();
            } 
            
            while(popsize > popedsize) {
                r = _queue.top();
                tlist.push_back(r);
                _queue.pop();
                popedsize++;
            }
            return popsize;
        }
        bool  empty() {return _queue.empty(); }
    private:
      priority_queue<Task*, vector<Task*>, taskcmp > _queue;
      Mutex                                          _mutex;
      
};
}
}
#endif
