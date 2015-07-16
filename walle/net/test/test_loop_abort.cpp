#include <walle/sys/wallesys.h>
#include <walle/net/EventLoop.h>

class testthread:public walle::sys::Thread {
	public:
		testthread(walle::net::EventLoop *loop):_loop(loop)
		{

		}
		virtual ~testthread() {}
		virtual void run()
		{
			_loop->loop();
		}
	walle::net::EventLoop *_loop;
};
int main ()
{
	walle::net::EventLoop loop;
	testthread abortThread(&loop);
	abortThread.start();
	while(1) {
		sleep(1);
	}
}
