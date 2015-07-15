#include <walle/net/IOComponent.h>
#include <sys/epoll.h>
#include <walle/net/ITask.h>
namespace walle {
namespace net{


class IOCTask:public ITask {
	public:
		IOCTask(void *data)
		{
			setUserData(data);
		}
		virtual void runTask()
		{
			IOComponent* ioc = (IOComponent*)_userData;
			ioc->update();
			ioc->decRef();
		}

};

const int IOComponent::kNone = 0;
const int IOComponent::kRead = EPOLLIN;
const int IOComponent::kWrite = EPOLLOUT;
AtomicInt64  IOComponent::gIocseq;


IOComponent::IOComponent(EventLoop *loop, EventFD * fd):
	_owner(loop),
	_evfd(fd),
	_stat(UNCONNECT),
	_ref(),
	_using(),
	_seqId(gIocseq.incrementAndGet()),
	_event(kNone),
	_revent(kNone),
	_inloop(false),
	_isServer(false),
	
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
void IOComponent::joinLoop(bool readon, bool writeon)
{
	assert(_inloop == false);
	assert(isUsing() == false);
	setUsing(true);
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
	_polleractivity = kNew;
	update();
	
}

void IOComponent::enableReadWrite(bool readon, bool writeon)
{
	assert(_inloop == true);
	assert(isUsing() == false);
	bool r = setUsing();
	assert(r == true);
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
		setUnusing();
		return ;
	}
	
	_polleractivity = kUpdate;
	update();
}
void IOComponent::detechLoop()
{
	assert(_inloop == true);
	assert(isUsing() == false);
	setUsing();
	_polleractivity = kDel;
	update();
}

void IOComponent::update()
{
	if(_owner->isInLoopThread()) {
		updateInloop();
	} else {
		_owner->inQueueIoc(this);
	}
}
void IOComponent::updateInloop() 
{
	if(_polleractivity == kNew) {
		_owner->addIoc(this,_event&kRead,_event&kWrite);
		_inloop = true;

	} else if(_polleractivity = kUpdate) {
		_owner->updateIoc(this,_event&kRead,_event&kWrite);
	} else if(_polleractivity == kDel ){
		_owner->removeIoc(this);
		_inloop = false;
	} else if(_polleractivity == kNoActivity) {
		//should never curr
	}
	
	setUnusing();
	return ;
}

}
}
