#include <walle/sys/wallesys.h>
#include <walle/net/Eventloopthread.h>
#include <walle/net/Tcpclient.h>

#include <boost/bind.hpp>

using namespace walle::sys;
using namespace walle::net;

int main(int argc, char* argv[])
{
  Logger::setLogLevel(Logger::DEBUG);

  EventLoopThread loopThread;
  {
  	EventLoop* loop = loopThread.startLoop();
  	AddrInet serverAddr("127.0.0.1", 22); // should succeed
  	TcpClient client(loop, serverAddr, "TcpClient");
  	client.connect();
  	Thread::sleep(1000*1000);// wait for connect
  	client.disconnect();
	//Thread::sleep(1000 * 1000);
  }

  Thread::sleep(1000 * 1000);
}

