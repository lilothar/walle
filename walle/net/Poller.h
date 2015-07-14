#ifndef WALLE_POLLER_H_
#define WALLE_POLLER_H_
#include <walle/sys/Time.h>
#include <sys/epoll.h>
#include <vector>


using walle::sys::Time;
namespace walle {

namespace net {
class EventLoop;
class IOComponent;
class EventFD;
class Poller{
    public:
        typedef vector<IOComponent* > IOCEventList;
    public:
        Poller(EventLoop *loop);
        ~Poller();
        bool init();
        bool addEvent(EventFD *fd, bool readon, bool writeon);
        bool setEvent(EventFD *fd, bool readon, bool writeon);
        void removeEvent(EventFD *fd);
        Time getEvents(IOCEventList &list, int timeoutms = 1);
   private:
      static const int kinitEventListSize = 32;
      typedef std::vector<struct epoll_event> EpollEventList;
      void  fillEventList(size_t num, IOCEventList &result);
   private:
      EventLoop      *_owner;
      int             _epfd;
      EpollEventList  _events;
    
    
};
}
}
#endif
