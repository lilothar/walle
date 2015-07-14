#ifndef WALLE_EVENTLOOPTHREAD_H_
#define WALLE_EVENTLOOPTHREAD_H_
#include <walle/sys/Thread.h>
#include <walle/sys/Mutex.h>
#include <walle/sys/Cond.h>

namespace walle {
namespace sys{
class EventLoopThread:public Thread{
	public:
		typedef void (*ThreadInitFunc)(void *args);
	public:
		EventLoopThread(ThreadInitFunc handler, void *args = NULL);

		virtual ~EventLoopThread();
		EventLoop* startLoop();
		void       stopLoop();

		virtual void run();
		EventLoop* getLoop();
		
	private:
		EventLoop *_loop;
		bool       _exit;
		Mutex      _mutex;
		Cond       _cond;
		void      *_initarg;
		ThreadInitFunc _initfunc;
};
}
}