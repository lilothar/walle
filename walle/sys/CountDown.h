#ifndef WALLE_COUNTDOWN_H_
#define WALLE_COUNTDOWN_H_
#include <walle/sys/Mutex.h>
#include <walle/sys/Cond.h>

namespace walle {

namespace sys {

class CountDown {
    public:
        explicit CountDown(int c);
        ~CountDown();
        void down();
        void wait();
        int getCount();
   private:
        Mutex _mutex;
        Cond  _cond;
        int   _count;
        
};

}
}
#endif
