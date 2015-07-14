#include <walle/sys/wallesys.h>

namespace walle {
namespace sys {
	EventLoopThread::EventLoopThread(ThreadInitFunc handler, void *args):
		_initfunc(handler),_initarg(args),_exit(false)
	{}
	
	EventLoopThread::~EventLoopThread()
	{
		if( running()) {
			_exit = true;
			_loop->quit();
			join();
		}
	}

	void EventLoopThread::run()
	{
		EventLoop loop;
		if(_initfunc) {
			_initfunc(_initarg);
		}
		{
			ScopeMutex mu(&_mutex);
			_loop = &loop;
			_cond.signal();
		}
		loop.loop();
		_loop = NULL;
	}
EventLoop* EventLoopThread::startLoop()
{
	assert(!running());
	start();
	while(_loop == NULL) {
		_cond.wait(&_mutex,1000);
	}
	return _loop;
}
	
	void EventLoopThread::stopLoop()
	{
		_exit = true;
		_loop->quit();
		join();
	}

}
}
