#include <walle/net/Tcpserver.h>

#include <walle/sys/Logging.h>
#include <walle/net/Acceptor.h>
#include <walle/net/Eventloop.h>
#include <walle/net/Eventloopthreadpool.h>
#include <walle/net/Socket.h>

#include <boost/bind.hpp>

#include <stdio.h>  // snprintf

using namespace walle::sys;
namespace walle {
namespace net {

TcpServer::TcpServer(EventLoop* loop,
                      AddrInet& listenAddr,
                     const string& nameArg,
                     Option option)
  : _loop(loop), 
    _hostport(listenAddr.toString()),
    _name(nameArg),
    _acceptor(new Acceptor(loop, listenAddr, option == kReusePort)),
    _threadPool(new EventLoopThreadPool(loop)),
    _connectionCallback(defaultConnectionCallback),
    _messageCallback(defaultMessageCallback),
    _nextConnId(1)
{
  _acceptor->setNewConnectionCallback(
      boost::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{
  _loop->assertInLoopThread();

  for (ConnectionMap::iterator it(_connections.begin());
      it != _connections.end(); ++it)
  {
    TcpConnectionPtr conn = it->second;
    it->second.reset();
    conn->getLoop()->runInLoop(
      boost::bind(&TcpConnection::connectDestroyed, conn));
    conn.reset();
  }
}

void TcpServer::setThreadNum(int numThreads)
{
  assert(0 <= numThreads);
  _threadPool->setThreadNum(numThreads);
}

void TcpServer::start()
{
  if (_started.getAndSet(1) == 0)
  {
    _threadPool->start(_threadInitCallback);

    assert(!_acceptor->listenning());
    _loop->runInLoop(
        boost::bind(&Acceptor::listen, get_pointer(_acceptor)));
  }
}

void TcpServer::newConnection(int sockfd, const AddrInet& peerAddr)
{
  _loop->assertInLoopThread();
  EventLoop* ioLoop = _threadPool->getNextLoop();
  char buf[128];
  snprintf(buf, sizeof buf, "%s:%s#%d",_name.c_str(), _hostport.c_str(), _nextConnId);
  ++_nextConnId;
  string connName(buf);
  AddrInet localAddr;
  localAddr.upDateLocal(sockfd);

  TcpConnectionPtr conn(new TcpConnection(ioLoop,
                                          connName,
                                          sockfd,
                                          localAddr,
                                          peerAddr));
  _connections[connName] = conn;
  conn->setConnectionCallback(_connectionCallback);
  conn->setMessageCallback(_messageCallback);
  conn->setWriteCompleteCallback(_writeCompleteCallback);
  conn->setCloseCallback(
      boost::bind(&TcpServer::removeConnection, this, _1)); 
  ioLoop->runInLoop(boost::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{

  _loop->runInLoop(boost::bind(&TcpServer::removeConnectionInLoop, this, conn)
);
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
  _loop->assertInLoopThread();
  size_t n = _connections.erase(conn->name());
  (void)n;
  assert(n == 1);
  EventLoop* ioLoop = conn->getLoop();
  ioLoop->queueInLoop(
      boost::bind(&TcpConnection::connectDestroyed, conn));
}
}
}
