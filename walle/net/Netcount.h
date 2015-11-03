#ifndef WALLE_NET_NETCOUNT_H_
#define WALLE_NET_NETCOUNT_H_

#include <walle/sys/Atomic.h>

namespace walle {
namespace  net{
    class NetCount {
    public:
        NetCount();
        ~NetCount();
        void log();
        void clear();
    
    public:
        walle::sys::AtomicInt64 _starttime;
        walle::sys::AtomicInt64 _dataReadCnt;    // # bytes read
        walle::sys::AtomicInt64 _dataWriteCnt;   // # bytes written
    
    public:
        static NetCount _gNetCount; // È«¾Ö
    
    };
    
#define DYNET_GLOBAL_STAT dlnet::NetCount::_gNetCount
#define DYNET_COUNT_DATA_READ(i) {DYNET_GLOBAL_STAT._dataReadCnt.add(i);}
#define DYNET_COUNT_DATA_WRITE(i) {DYNET_GLOBAL_STAT._dataWriteCnt.add(i);}

}
}
#endif
