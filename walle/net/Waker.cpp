#include <walle/net/Waker.h>
#include <walle/net/EventLoop.h>
#include <boost/bind.hpp>

namespace walle {
namespace net{
	int createEventfd()
	{
  		int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  		if (evtfd < 0) {
    		LOG_ERROR<<"Failed in eventfd";
    		abort();
  		}
  		return evtfd;
}
	Waker::Waker(EventLoop* loop):_loop(loop),_evfd(createEventfd()),_channel(ChannelPtr(new Channel(_loop,_evfd)))
	{
	

	}
	Waker::~Waker()
	{
		::close(_evfd);
	}
	
	ChannelPtr Waker::channel() 
	{ 
		return ChannelPtr(_channel); 
	}
	void Waker::wakeUp()
	{
		uint64_t one = 1;
  		ssize_t n = ::write(_evfd, &one, sizeof one);
  		if (n != sizeof one) {
    		LOG_ERROR<<"EventLoop::wakeup() writes "<<n<<" bytes instead of 8";
  		}	
	}
	void Waker::handRead(Time t)
	{
		uint64_t one = 1;
  		ssize_t n = ::read(_evfd, &one, sizeof one);
  		if (n != sizeof one) {
    		LOG_ERROR<<"EventLoop::wakeup() reads "<<n<<" bytes instead of 8";
  		}
	}
	void Waker::start()
	{
		ChannelPtr guard(_channel);
		guard->tie(shared_from_this());
		guard->setReadCallback(boost::bind(&Waker::handRead,this,_1));
		guard->enableReading();
	}
	void Waker::stop()
	{
		ChannelPtr guard(_channel);
		_loop->runInLoop(boost::bind(&Channel::disableAll, guard));
		_loop->runInLoop(boost::bind(&Channel::remove, guard));
	}


}
}

