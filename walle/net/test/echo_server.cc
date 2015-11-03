#include <walle/net/wallenet.h>

#include <walle/sys/wallesys.h>
#include <walle/algo/functional.h>

#include <utility>

#include <stdio.h>
#include <unistd.h>
#include <iostream>
using namespace std;
using namespace walle::sys;
using namespace walle::net;

int numThreads = 0;
AtomicInt64 sendcount;
class EchoServer
{
 public:
  EchoServer(EventLoop* loop,  AddrInet& listenAddr)
    : _loop(loop),
      _server(loop, listenAddr, "EchoServer")
  {
    _server.setConnectionCallback(
        std::bind(&EchoServer::onConnection, this, std::_1));
    _server.setMessageCallback(
        std::bind(&EchoServer::onMessage, this, std::_1, std::_2, std::_3));
    _server.setThreadNum(numThreads);
  }

  void start()
  {
    _server.start();
  }
  // void stop();

 private:
  void onConnection(const TcpConnectionPtr& conn)
  {
    LOG_TRACE << conn->peerAddress().toString() << " -> "
        << conn->localAddress().toString() << " is "
        << (conn->connected() ? "UP" : "DOWN");
    LOG_INFO << conn->getTcpInfoString();

   // conn->send("hello\n");
  }

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Time time)
  {
    string msg(buf->retrieveAllAsString());
    LOG_TRACE << conn->name() << " recv " << msg.size() << " bytes at " << time.toDateTime();
    /*if (msg == "exit\n")
    {
      conn->send("bye\n");
      conn->shutdown();
    }
    if (msg == "quit\n")
    {
      loop_->quit();
    }
    */
    conn->send(msg);
	sendcount.add(msg.length());
  }

  EventLoop* _loop;
  TcpServer _server;
};
void printcount()
{
	cout<<sendcount.get()<<endl;
}
int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid() << ", tid = " << walle::LocalThread::tid();
  //LOG_INFO << "sizeof TcpConnection = " << sizeof(TcpConnection);
  if (argc > 1)
  {
    numThreads = atoi(argv[1]);
  }
  EventLoop loop;
  AddrInet listenAddr("0.0.0.0", 2000);
  EchoServer server(&loop, listenAddr);

  server.start();
  loop.runEvery(3*1000*1000,printcount);
  loop.loop();
}

