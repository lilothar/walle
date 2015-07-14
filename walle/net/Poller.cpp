#include <walle/net/Poller.h>

namespace walle{
namespace net{

Poller::Poller(EventLoop* loop):
	_owner(loop),
	_epfd(-1),
	_events()
{
	
}
bool Poller::init()
{
	_events.reserv(kinitEventListSize);
	_epfd = ::epoll_create1(EPOLL_CLOEXEC);
	if (_epfd <= 0) {
			return false;
		}
		return true;
}

Poller::~Poller()
{
	::close(_epfd);
}

bool Poller::addEvent(EventFD *fd, bool readon, bool writeon)
{
	int efd = fd->getHandle();
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.data.ptr = fd->getIoc();
	ev.events = 0;

	if (readon) {
		ev.events |= EPOLLIN;
	}
	if (writeon) {
		ev.events |= EPOLLOUT;
	}

	bool rc = (epoll_ctl(_epfd, EPOLL_CTL_ADD, efd, &ev) == 0);
	return rc;
	
}

bool Poller::setEvent(EventFD *fd, bool readon, bool writeon) 
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.data.ptr = fd->getIoc();
	ev.events = 0;

	if (readon) {
	  ev.events |= EPOLLIN;
	 }
	 if (writeon) {
	  ev.events |= EPOLLOUT;
	}
	 

	bool rc = (epoll_ctl(_epfd, EPOLL_CTL_MOD, fd->getHandle(), &ev) == 0);
	return rc;
}

bool Poller::removeEvent(EventFD *fd)
{
	struct epoll_event ev;
	::memset(&ev, 0, sizeof(ev));
	ev.data.ptr = fd->getIoc();
	ev.events = 0;
	bool rc = (epoll_ctl(_epfd, EPOLL_CTL_DEL, fd->getHandle(), &ev) == 0);
	return rc;
}
Time Poller::getEvents(IOCEventList &list, int timeoutms = 1)
{
	int numEvents = ::epoll_wait(_epfd,
                               &*_events.begin(),
                               static_cast<int>(_events.size()),
                               timeoutms);
	int saveerror = errno;
	Time now = Time::now();
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

void Poller::fillEventList(size_t num, IOCEventList &result)
{
	for (size_t i = 0; i < num; i++) {
		
		IOComponent *ioc = _events[i].data.ptr;
		ioc->setRevent(int _events[i].events);
		result.push_back(ioc);
	}	
}

}
}
