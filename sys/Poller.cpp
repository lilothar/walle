#include <walle/net/Poller.h>
#include <walle/net/EventLoop.h>

namespace walle{
namespace sys{
	Poller::Poller(EventLoop *loop):_owner(loop)
	{

	}

	bool Poller::init()
	{
		_events.reserv(kinitEventListSize);
		_epollfd = ::epoll_create1(EPOLL_CLOEXEC);
		if (_epollfd <= 0) {
			return false;
		}
		return true;
	}
	Poller::~Poller()
	{
		::close(_epollfd);
	}
	bool Poller::addEvent(EventFD *fd, bool readon, bool writeon)
	{
		int efd = fd->getHandler();
		struct epoll_event ev;
    	memset(&ev, 0, sizeof(ev));
    	ev.data.ptr = fd->getIOComponent();
		ev.events = 0;

		if (readon) {
        	ev.events |= EPOLLIN;
    	}
    	if (writeon) {
        	ev.events |= EPOLLOUT;
    	}

		bool rc = (epoll_ctl(_epollfd, EPOLL_CTL_ADD, efd, &ev) == 0);
		return rc;
		
	}
	
	bool Poller::setEvent(EventFD *fd, bool readon, bool writeon) 
	{
		struct epoll_event ev;
    	memset(&ev, 0, sizeof(ev));
    	ev.data.ptr = fd->getIOComponent();
    	ev.events = 0;

    	if (readon) {
      	  ev.events |= EPOLLIN;
   		 }
   		 if (writeon) {
      	  ev.events |= EPOLLOUT;
    	}
		 

    	bool rc = (epoll_ctl(_epollfd, EPOLL_CTL_MOD, fd->getSocketHandle(), &ev) == 0);
    	return rc;
	}

	
	bool Poller::removeEvent(EventFD *fd)
	{
		struct epoll_event ev;
    	memset(&ev, 0, sizeof(ev));
    	ev.data.ptr = fd->getIOComponent();
    	ev.events = 0;
    	bool rc = (epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd->getSocketHandle(), &ev) == 0);
	    return rc;
	}
	Time Poller::getEvents(IOEventList &list,int timeoutms )
	{
		int numEvents = ::epoll_wait(_epollfd,
                               &*_events.begin(),
                               static_cast<int>(_events.size()),
                               timeoutms);
		int saveerror = errno;
		Time now(Time::now());
		if(numEvents > 0) {
			fillEventList(numEvents, list);
			if(numEvents == static_cast<int>(_events.size())) {
				_events.resize(numEvents * 2);	
			}
		} else if(numEvents = 0) {
			if(saveerror != EINTR) {
				errno = saveerror;
			}
		}
		return now;
		
	}

	void Poller::fillEventList(int num, IOEventList &list)
	{
		IOEvent dummy;
		dummy._type = IOEvent::EventPoll;
		list->resize(num, dummy);
		for(int i = 0 ; i< num; i++) {
			IOComponet *ioc = _events[i].data.ptr;
			list[i]._ioc = ioc;
			 if (_events[i].events & (EPOLLERR | EPOLLHUP)) {
            	list[i]._erroron = true;
        	 }
        	if ((_events[i].events & EPOLLIN) != 0) {
            	list[i]._readon = true;
        	}
        	if ((_events[i].events & EPOLLOUT) != 0) {
           	 list[i]._writeon = true;
        	}
		}
	}



}
}
