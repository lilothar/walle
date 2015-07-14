#ifndef WALLE_POLLER_H_
#define WALLE_POLLER_H_
#include <walle/sys/EventFD.h>
#include <walle/sys/IOComponent.h>
#include <vector>

namespace walle{
namespace sys{

class EventLoop;
class IOEvent {
public:
    enum EventType{
        EventNone = 0,
        EventAdd,
        EventUpdate,
        EventDel,
        EventPoll          
    };
public:
    IOEvent():
        _readon(false),
        _writeon(false),
        _erroron(false),
        _type(EventNone),
        _ioc(0)
    {}
    void reset() 
    {
        _readon = false;
        _writeon = false;
        _erroron = false;
        _type = EventNone;
        _ioc = NULL;
    }
  bool _readon;
  bool _writeon;
  bool _erroron;
  EventType _type;
  IOComponet *_ioc;
};

typedef vector<IOEvent> IOEventList;

class Poller {
    public:
        Poller(EventLoop *loop);
        ~Poller();
        bool init();
        bool addEvent(EventFD *fd, bool readon, bool writeon) ;
        bool setEvent(EventFD *fd, bool readon, bool writeon) ;
        bool removeEvent(EventFD *fd) ;
        Time getEvents(IOEventList &list,int timeoutms = 1) ;
   private:
        static const int kinitEventListSize = 32;
        void fillEventList(int num, IOEventList &list);
       typedef std::vector<struct epoll_event> EventList;

       EventList _events;
       int       _epollfd;
       EventLoop *_owner;

};
}
}
#endif
