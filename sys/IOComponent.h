#ifndef WALLE_IOCOMPONET_H_
#define WALLE_IOCOMPONET_H_
#include <walle/sys/Atomic.h>
#include <stdint.h>

using walle::sys::AtomicInt32;
using walle::sys::AtomicInt64;


namespace walle {
namespace sys{
class EventLoop;
class EventFD;
class IOComponent{
    public:
        enum stat{
            UNCONNECT,
            CONNECTTING,
            CONNECTED,
            CLOSED
        };
    public:
       IOComponent(EventLoop *loop, EventFD *FD);
       virtual ~IOComponent();

       virtual bool init(bool isserver = false) = 0;
       
       virtual bool handwrite(const Time& t ) = 0;
       virtual bool handread(const Time& t) = 0;
       virtual bool handerror(const Time& t) = 0;
       virtual bool enableWrite(bool onwrite);
       virtual void close();
       void         incRef() { _ref.increment(); }
       void         decRef(){ _ref.decrement(); }
       int          getRef() { return _ref.get(); }
       EventLoop*   getOwner() { return _owner; }
       EventFD*     getEventFD() { return _evfd; }
       void         setStat(int stat) { _stat = stat; }
       int          getStat(){ return _stat; }
       bool         isUsed() { return _used; }
       void         setUsed(bool on) { _used = on; }
       bool         joinLoop(bool readon, bool writeon);
       bool         detachLoop();
       int64_t      getId() {return _id; }
       
       
    protected:
       EventLoop    *_owner;
       EventFD      *_evfd;
       int           _stat;
       AtomicInt32   _ref;
       bool          _used;
       int64_t       _id;
 
       static AtomicInt64 gseq;
};

}
}
#endif
