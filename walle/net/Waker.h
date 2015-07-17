#ifndef WALLE_WAKER_H_
#define WALLE_WAKER_H_
#include <sys/eventfd.h>
#include <walle/sys/wallesys.h>
#include <walle/net/Channel.h>
#include <boost/enable_shared_from_this.hpp>

using namespace walle::sys;
namespace walle {
namespace net {
class EventLoop;
class Waker:public boost::enable_shared_from_this<Waker>{
	public:
		Waker(EventLoop* loop);
		~Waker();

		ChannelPtr channel();
		void wakeUp();
		void start();
		void stop();
	private:
		void handRead(Time t);
		EventLoop   *_loop;
		int         _evfd;
		WeakChannel _channel;

};
}
}
#endif
