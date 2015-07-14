#include <walle/net/IOComponent.h>
#include <sys/epoll.h>

namespace walle {
namespace net{
const int IOComponent::kNone = 0;
const int IOComponent::kRead = EPOLLIN;
const int IOComponent::kWrite = EPOLLOUT;
AtomicInt64  IOComponent::gIocseq;


IOComponent::IOComponent(EventLoop *loop, EventFD * fd):
	_owner(loop),
	_evfd(fd),
	_stat(UNCONNECT),
	_ref(),
	_using(false),
	_seqId(gIocseq.incrementAndGet()),
	_event(kNone),
	_revent(kNone),
	_writeEnabled(false),
	_readEnabled(false),
	_addToLoop(false)	
	
{
	_evfd->setIoc(this);
}
IOComponent::~IOComponent() 
{
	if(_evfd) {
		_evfd->close();
		delete _evfd;
		_evfd = NULL;
	}
}
void IOComponent::enableReadWrite(bool readon, bool writeon)
{
	int et = _event;
	if(readon) {
		et |= kRead;
	} else {
		et &= ~kRead;
	}

	if ( writeon) {
		et |= kWrite;
	} else {
		et &= ~kWrite;
	}

	if(et == _event) {
		return ;
	}
	update();
}

void IOComponent::update()
{
	
}

}
}
