#include <walle/sys/wallesys.h>
#include <sys/eventfd.h>

namespace walle {
namespace LocalThread{
	
	__thread walle::sys::EventLoop *t_loopInthread = 0;
	
}



namespace sys {
static EventLoop *getThreadLoop()
{
	return LocalThread::t_loopInthread;

}
int createEventfd()
{
	int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  	if (evtfd < 0)
  	{
    	abort();
  	}
  	return evtfd;
}
class WakeUpEvent:public EventFD{
	public:
		WakeUpEvent()
		{
					
		}
		virtual ~WakeUpEvent()
		{
			
		}
		
		virtual int read(char *data, int len)
		{
			return ::read(_fd,data,len);
		}

        virtual int write(char* data, int len)
        {
			return ::write(_fd,data,len);
		}
		
};
class WakeUpIoc:public IOComponet{
	public:
	WakeUpIoc(EventLoop*loop, EventFD *fd):IOComponet(loop,fd){}
	virtual ~WakeUpIoc(){}
	virtual bool init(bool isserver = false){ return true; }
	virtual bool handread(const Time & t)
	{
		int64_t one ;
		size_t readnum;
		readnum = _evfd->read((char*)&one,sizeof(one));
		if(readnum != sizeof(one)) {
			return false;
		}
		return true;
	}
	virtual bool handwrite(const Time & t){ return true ; }
		
	virtual bool handerror(const Time & t){ return true ; }
};
EventLoop::EventLoop():_runnig(false),
	_quit(false),
	_thid (LocalThread::tid()),
	_wakeUpIoc(0)
{
	_poller = new Poller(this);
	int fd =createEventfd();
	if (fd < 0) {
		abort();
	}
	EventFD *evfd = new WakeUpEvent();
	evfd->setFd(fd);
	_wakeUpIoc  = new WakeUpIoc(this, evfd);
	_wakeUpIoc->joinLoop(true,false);
	LocalThread::t_loopInthread = this;
	
}

EventLoop::~EventLoop()
{

}
void EventLoop::runInloop(Task *task,  bool ignorePriority)
{
	if(ignorePriority && isInLoop()) {
		doTaskInloop(task);
	} else {
		_taskQueue.push(task);
	}
}		

bool EventLoop::isInLoop()
{
	return _thid == walle::LocalThread::tid();
}
void EventLoop::updateIoc(IOComponet *ioc, bool writeon, bool readon)
{
	IOEvent dummy;
	dummy._type = IOEvent::EventUpdate;
	dummy._ioc = ioc;
	dummy._readon = readon;
	dummy._writeon = writeon;
	if (isInLoop()) {
		doIocTaskInLoop(dummy);
	} else {
		ScopeMutex mu(&_mutex);
		_delayOptTask.push_back(dummy);
	}
		
	
	
}
void EventLoop::wakeUp()
{
	int64_t one = 1;
	int writenum;
	writenum = _wakeUpIoc->getEventFD()->write((char*)&one,sizeof(one));
	

}

void EventLoop::processIoEvent(IOEvent &ev)
{
	if (ev._readon) {
		ev._ioc->handread(_lastPollTime);
	} else if (ev._writeon) {
		ev._ioc->handwrite(_lastPollTime);
	} else if (ev._erroron) {
		ev._ioc->handerror(_lastPollTime);
	}
}

void EventLoop::loop()
{
	
	assert(!_runnig);
	_runnig = true;
	_quit = false;
	
	while(!_quit) {

		_activeIoc.clear();
		_lastPollTime = _poller->getEvents(_activeIoc);
		int ionum = _activeIoc.size();
		for(int i = 0; i < ionum ; i++) {
			processIoEvent(_activeIoc[i]);	
		}

		dodelayOptTask();
		doPriorityTask();
		
	}
}

void EventLoop::doIocTaskInLoop(const IOEvent &ioe)
{
	bool rc = true;
	if (ioe._type == IOEvent::EventAdd) {
		rc = _poller->addEvent(ioe._ioc->getEventFD() , ioe._readon, ioe._writeon);
		_iocMap[ioe._ioc->getId()] = ioe._ioc;
	}else if (ioe._type == IOEvent::EventUpdate) {
		rc = _poller->setEvent(ioe._ioc->getEventFD() , ioe._readon, ioe._writeon);
	} else if(ioe._type == IOEvent::EventDel) {
		rc = _poller->removeEvent(ioe._ioc->getEventFD());
		_iocMap.find(ioe._ioc->getId());
	}
	if(!rc) { 
		ioe._ioc->handerror(Time::now());
	}
}

void EventLoop::quit()
{
	_quit == true;
	wakeUp();
}

void EventLoop::dodelayOptTask()
{
	IOEventList mylist;
	{
		ScopeMutex mu(&_mutex);
		mylist.swap(_delayOptTask);
	}
	
	int optnum = mylist.size();
	for (int i = 0; i < optnum; i++) {
		doIocTaskInLoop(mylist[i]);
	}
}
void EventLoop::doTaskInloop(Task *t)
{
	assert(isInLoop());
	t->runTask();
}
void EventLoop::doPriorityTask()
{
	int tasknum;
	TaskQueue::TaskList mylist;
	mylist.clear();
	tasknum = _taskQueue.multiPop(mylist);
	if(tasknum == 0) {
		return ;
	}
	for (int i = 0; i < tasknum; i++ ) {
		doTaskInloop(mylist[i]);	
	}	
	
}
bool EventLoop::assertInloop()
{
	if(!isInLoop()) {
		assert(false);
	}
}

void EventLoop::addIoc(IOComponet *ioc, bool writeon, bool readon) 
{
	IOEvent dummy;
	dummy._type = IOEvent::EventAdd;
	dummy._ioc = ioc;
	dummy._readon = readon;
	dummy._writeon = writeon;
	if (isInLoop()) {
		doIocTaskInLoop(dummy);
	} else {
		ScopeMutex mu(&_mutex);
		_delayOptTask.push_back(dummy);
	}
		
}
void EventLoop::removeIoc(IOComponet *ioc)
{
		bool ret;
	IOEvent dummy;
	dummy._type = IOEvent::EventAdd;
	dummy._ioc = ioc;
	if (isInLoop()) {
		doIocTaskInLoop(dummy);
	} else {
		ScopeMutex mu(&_mutex);
		_delayOptTask.push_back(dummy);
	}

}

}
}
