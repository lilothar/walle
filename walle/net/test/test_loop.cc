#include <walle/net/Eventloop.h>

walle::net::EventLoop *loop = NULL;
int i;
walle::net::TimerWheelHander gptr;

void printI()
{
	printf("I am %dth\n",i++);
	if(i > 10 ) {
		gptr.release();
		loop->quit();
		return;
	}
}
int main()
{
	walle::net::EventLoop mainloop;
	loop = &mainloop;
	i = 0;
	gptr = loop->runEverySec(static_cast<int64_t>(1),std::bind(printI));
	loop->loop();
	return 0;

}
