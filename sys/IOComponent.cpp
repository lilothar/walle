#include <walle/sys/wallesys.h>

namespace walle {

namespace sys {

AtomicInt64 IOComponent::gseq;

IOComponent::IOComponent(EventLoop *loop, EventFD *FD):_owner(loop),
		_evfd(FD),_id(gseq.incrementAndGet())
{
	assert(_owner);
	assert(_evfd);
	_evfd->setIoc(this);
	_used = false;
	_stat = UNCONNECT;
}

IOComponent::~IOComponent()
{
	if(_evfd) {
		_evfd->close();
		delete _evfd;
		_evfd = NULL;
	}
}
bool IOComponent::enableWrite(bool onwrite)
{
	_owner->updateIoc(this, onwrite,true);
	return true;
}

bool IOComponent::joinLoop(bool readon, bool writeon)
{
	 _owner->addIoc(this, writeon, readon);
	 return true;
}
bool IOComponent::detachLoop()
{
	_owner->removeIoc(this);
	return true;
	
}


}
}
