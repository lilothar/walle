#include <walle/net/EventLoop.h>
namespace walle {
namespace LocalThread {

	__thread walle::net::EventLoop *t_EventLoop;
}
namespace net {
	

EventLoop::EventLoop():
	_taskQueue(),
	_mytask(),
	_quit(false),
	_thid(walle::LocalThread::tid())
{
	walle::LocalThread::t_EventLoop = this;
	LOG_INFO<<_thid;
	LOG_INFO<<walle::LocalThread::tid();
	LOG_INFO<<pthread_self();
	
}
EventLoop::~EventLoop()
{
	assertInloop();
	walle::LocalThread::t_EventLoop = NULL;

	_taskQueue.popAll(_mytask);
	for(size_t i = 0; i <_mytask.size(); i++) {
		delete _mytask[i];
	}
	
}
bool EventLoop::isInLoopThread()
{
	return _thid == walle::LocalThread::tid();
}
EventLoop* EventLoop::getLocalEventLoop()
{
	return 	walle::LocalThread::t_EventLoop;
}

void EventLoop::runTask(ITask *task)
{
	if(!task) {
		LOG_WARN<<"a null type task";
	}
	if (isInLoopThread()) {
		task->runTask();
	} else {
		_taskQueue.push(task);
	}
}
void EventLoop::loop()
{
	assertInloop();
	while(!_quit) {
		
		doTaskInloop();
		
	}
}

void EventLoop::doTaskInloop()
{
	assertInloop();
	_taskQueue.popAll(_mytask);
	if(_mytask.empty()) {
		return;
	}

	for(size_t i = 0; i <_mytask.size(); i++) {
		_mytask[i]->runTask();
		delete _mytask[i];
	}
	_mytask.clear();
}

void EventLoop::assertInloop()
{
	if(isInLoopThread()) {
		return;
	}
	LOG_ERROR<<_thid<<" "<<walle::LocalThread::tid();
	LOG_ERROR<<"not in loop thread";
	assert(false);
}


}
}
