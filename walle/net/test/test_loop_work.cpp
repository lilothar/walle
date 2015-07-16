#include <walle/sys/wallesys.h>
#include <walle/net/EventLoop.h>
#include <walle/net/ITask.h>
#include <iostream>
using namespace std;
class testthread:public walle::sys::Thread {
	public:
		testthread():_loop(0)
		{
		}
		virtual ~testthread() {}
		virtual void run()
		{
			
			walle::net::EventLoop loop;
			{
				walle::sys::ScopeMutex lock(&_gmutex);
				_loop = &loop;
				_gcond.broadcast();
			}
			_loop->loop();
		}
		void startloop()
		{
			assert(!running());

			start();
			if(_loop == NULL) {
				walle::sys::ScopeMutex lock(&_gmutex);
				_gcond.wait(&_gmutex);
			}
		}
	walle::net::EventLoop* getloop() { return _loop; }
	private:
	walle::net::EventLoop *_loop;
	walle::sys::Mutex _gmutex;
	walle::sys::Cond _gcond;
};
walle::sys::AtomicInt32 gnum;
class printTask:public walle::net::ITask{
	public:
	printTask():ITask() {}
	virtual ~printTask() {}
	virtual void runTask() 
	{
		LOG_ERROR<<walle::LocalThread::tid()<<" Task runing";
		gnum.decrement();
	}
};
int main ()
{

	walle::net::EventLoop *loop = NULL;
	testthread workThread;
	LOG_WARN<<"main " <<walle::LocalThread::tid();
	LOG_WARN<<"main start...";
	workThread.startloop();
	loop = workThread.getloop();
	walle::net::ITask *task;
	while(1) {
		if(gnum.get()>10000) {
			sleep(1);
			continue;
		}
		task = new printTask;
		//task->runTask();
		loop->runTask(task);
		gnum.increment();
		//sleep(1);
	}
}

