#include <walle/net/EventLoop.h>
#include <walle/net/TimerTask.h>
namespace walle {

namespace net {

bool EventLoop::addIoc(IOComponent*ioc, bool readon, bool writeon)
{
	_poller->addEvent(ioc->getEventFD(), readon, writeon);
}

void EventLoop::inQueueIoc(IOComponent *ioc)
{
	_delayedIoc.push(ioc);
}
bool EventLoop::updateIoc(IOComponent*ioc, bool readon, bool writeon)
{
	_poller->setEvent(ioc->getEventFD(), readon, writeon);
}
void EventLoop::removeIoc(IOComponent*ioc)
{
	_poller->removeEvent(ioc->getEventFD());
}

EventLoop()
{

}
~EventLoop()
{

}
bool EventLoop::isInLoopThread()
{
	return _thid == walle::LocalThread::tid();
}
void EventLoop::addTimer(TimerTask *task)
{
	if(isInLoopThread()) {
		_timer->addTask(task);
	} else {
		_timerTaskqueue.push(task);
	}
}
void EventLoop::wakeUp();
void EventLoop::runTask(ITask *task)
{
	if(isInLoopThread()) {
		task->runTask();
	} else {
		_taskQueue.push(task);
	}
}

void EventLoop::doDelayedIocOpt()
{
	_myIoc.clear();
	size_t queuesize = _delayedIoc.popAll(_myIoc);
	for(size_t i = 0; i < queuesize; i++) {
		IOComponent *ioc = _myIoc[i];
		ioc->update();
	}
}
void EventLoop::dotaskInloop() 
{
	_mytask.clear();
	size_t queuesize = _delayedIoc.popAll(_myIoc);
	for(size_t i = 0; i < queuesize; i++) {
		ITask *task = _mytask[i];
		task->runTask();
	}
}

void EventLoop::loop()
{
	while(!_quit) {
		doDelayedIocOpt();	
		dotaskInloop();

	}
}

void EventLoop::runTimerTaskInloop()
{

}

}
}
