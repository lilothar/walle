#include <walle/net/Tcpclient.h>
#include <walle/sys/Logging.h>
#include <walle/net/Connector.h>
#include <walle/net/Eventloop.h>
#include <walle/net/Socket.h>

#include <walle/algo/functional.h>

#include <stdio.h>  // snprintf


using namespace walle::sys;

// TcpClient::TcpClient(EventLoop* loop)
//   : loop_(loop)
// {
// }

// TcpClient::TcpClient(EventLoop* loop, const string& host, uint16_t port)
//   : loop_(CHECK_NOTNULL(loop)),
//     serverAddr_(host, port)
// {
// }

using namespace walle::net;
namespace walle{
namespace util {
void removeConnection(EventLoop* loop, const TcpConnectionPtr& conn)
{
  loop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}

void removeConnector(const ConnectorPtr& connector)
{
	(void)connector;
  //connector->
}
}
namespace net {
TcpClient::TcpClient(EventLoop* loop,
                      AddrInet& serverAddr,
                     const string& name)
  : _loop(loop),
  	_serveraddr(serverAddr),
    _connector(new Connector(loop, serverAddr)),
    _name(name),
    _connectionCallback(defaultConnectionCallback),
    _messageCallback(defaultMessageCallback),
    _retry(false),
    _connect(false),
    _nextConnId(1)
{
	LOG_DEBUG<<"TcpClient::TcpClient ["<<_name<<"]";
  _connector->setNewConnectionCallback(
      std::bind(&TcpClient::newConnection, this, std::_1));
}

TcpClient::~TcpClient()
{
	LOG_DEBUG<<"TcpClient::~TcpClient ["<<_name<<"]";

  TcpConnectionPtr conn;
  bool unique = false;
  {
    ScopeMutex lock(&_mutex);
    unique = _connection.unique();
    conn = _connection;
  }
  if (conn) {
    assert(_loop == conn->getLoop());
    // FIXME: not 100% safe, if we are in different thread
    CloseCallback cb = std::bind(&util::removeConnection, _loop, std::_1);
    _loop->runInLoop(
        std::bind(&TcpConnection::setCloseCallback, conn, cb));
    	if (unique) {
      		 conn->forceClose();
    	}
  } else {
    	_connector->stop();
 	   _loop->runAfter(1, std::bind(&util::removeConnector, _connector));
  }
}

void TcpClient::connect()
{
	LOG_DEBUG<<"TcpClient::connect ["<<_name<<"]";
  _connect = true;
  _connector->start();
}

void TcpClient::disconnect()
{
	LOG_DEBUG<<"TcpClient::disconnect ["<<_name<<"]";

  _connect = false;

  {
    ScopeMutex lock(&_mutex);
    if (_connection)
    {
      _connection->shutdown();
    }
  }
}

void TcpClient::stop()
{
	LOG_DEBUG<<"TcpClient::stop ["<<_name<<"]";
  _connect = false;
  _connector->stop();
}

void TcpClient::newConnection(int sockfd)
{
	LOG_DEBUG<<"TcpClient::newConnection ["<<_name<<"]";
  _loop->assertInLoopThread();
  char buf[32];
  snprintf(buf, sizeof buf, ":%s#%d", _serveraddr.toString().c_str(), _nextConnId);
  ++_nextConnId;
  string connName = _name + buf;

  AddrInet localAddr;
  localAddr.upDateLocal(sockfd);
  // FIXME poll with zero timeout to double confirm the new connection
  // FIXME use make_shared if necessary
  TcpConnectionPtr conn(new TcpConnection(_loop,
                                          connName,
                                          sockfd,
                                          localAddr,
                                          _serveraddr));

  conn->setConnectionCallback(_connectionCallback);
  conn->setMessageCallback(_messageCallback);
  conn->setWriteCompleteCallback(_writeCompleteCallback);
  conn->setCloseCallback(
      std::bind(&TcpClient::removeConnection, this, _1)); // FIXME: unsafe
  {
    ScopeMutex lock(&_mutex);
    _connection = conn;
  }
  conn->connectEstablished();
}

void TcpClient::removeConnection(const TcpConnectionPtr& conn)
{
	LOG_DEBUG<<"TcpClient::removeConnection ["<<_name<<"]";
  _loop->assertInLoopThread();
  assert(_loop == conn->getLoop());

  {
    ScopeMutex lock(&_mutex);
    assert(_connection == conn);
    _connection.reset();
  }

  _loop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
  if (_retry && _connect)
  {
    _connector->restart();
  }
}
}
}
