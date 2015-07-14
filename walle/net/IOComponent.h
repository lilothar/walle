#ifndef WALLE_IOCOMPONENT_H_
#define WALLE_IOCOMPONENT_H_

#include <walle/sys/Time.h>
#include <walle/sys/Atomic.h>
#include <walle/net/EventFD.h>
#include <stdint.h>


namespace walle {
namespace net{
class EventLoop;
using walle::sys::AtomicInt32;
class IOComponent{
    public:
      enum IOCStatus{
          UNCONNECT,
          CONNECTTING,
          CONNECTED,
          CLOSED
      };
      enum PollerStatus{
            kNew,
            kUpdate,
            kDel
      };

      IOComponent(EventLoop *loop, EventFD * fd);
      virtual ~IOComponent();
      virtual void init(bool isserver = false ) = 0;
      virtual bool handRead(const Time &t) = 0;
      virtual bool handwrite(const Time &t) = 0;
      virtual bool handError(const Time &t) = 0;
      
      void enableReadWrite(bool readon, bool writeon);

	  void joinLoop(bool readon, bool writeon);
	  void detechLoop();
      
      void incRef() { _ref.increment(); }
      void decRef() { _ref.decrement(); }
      const EventLoop *getLoop() const  { return _owner; }
      const EventFD   *getEventFD() const { return _evfd; }
      void setEvent(const int &ev) { _event = ev; }

      int  getEvent() const { return _event; }

      void setRevent(int rev) { _revent = rev; }
      int  getRevent() { return _revent; }
      
      void setStat(const IOCStatus& stat) { _stat = stat; }

      IOCStatus getStat() const { return _stat; }
      bool isUsing() const { return _using.get() == 1 ? true : false ; }
      void setUsing(bool on) { on == true ?_using.getAndSet(1): _using.getAndSet(0); }
      int64_t getId() { return _seqId; }
    protected:
        void update();  
    protected:
        const EventLoop    *_owner;
        const EventFD      *_evfd;
        IOCStatus           _stat;
        AtomicInt32         _ref;
        AtomicInt32         _using;
        int64_t             _seqId;
		PollerStatus        _polleractivity;
        int                 _event;
        int                 _revent;
        bool                _writeEnabled;
        bool                _readEnabled;
        bool                _inloop;
		bool                _isServer;
        static AtomicInt64  gIocseq;
        static const int    kNone;
        static const int    kRead;
        static const int    kWrite;
        
};
}
}
#endif
