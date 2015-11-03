#ifndef WALLE_NET_TCPCLIENT_H_
#define WALLE_NET_TCPCLIENT_H_
#include <walle/algo/noncopyable.h>
#include <walle/algo/memory.h>
#include <walle/sys/Mutex.h>
#include <walle/net/TcpConnection.h>
#include <walle/net/Addrinet.h>
using namespace walle::sys;
namespace walle {
namespace net{

class Connector;
typedef std::shared_ptr<Connector> ConnectorPtr;

class TcpClient : std::noncopyable
{
 public:
  TcpClient(EventLoop* loop,
             AddrInet& serverAddr,
            const string& name);
  ~TcpClient();  // force out-line dtor, for scoped_ptr members.

  void connect();
  void disconnect();
  void stop();

  TcpConnectionPtr connection() 
  {
    ScopeMutex lock(&_mutex);
    return _connection;
  }

  EventLoop* getLoop() const { return _loop; }
  bool retry() const;
  void enableRetry() { _retry = true; }

  /// Set connection callback.
  /// Not thread safe.
  void setConnectionCallback(const ConnectionCallback& cb)
  { _connectionCallback = cb; }

  /// Set message callback.
  /// Not thread safe.
  void setMessageCallback(const MessageCallback& cb)
  { _messageCallback = cb; }

  /// Set write complete callback.
  /// Not thread safe.
  void setWriteCompleteCallback(const WriteCompleteCallback& cb)
  { _writeCompleteCallback = cb; }

 private:
  /// Not thread safe, but in loop
  void newConnection(int sockfd);
  /// Not thread safe, but in loop
  void removeConnection(const TcpConnectionPtr& conn);

  EventLoop* _loop;
  AddrInet _serveraddr;
  ConnectorPtr _connector; // avoid revealing Connector
  const string _name;
  ConnectionCallback _connectionCallback;
  MessageCallback _messageCallback;
  WriteCompleteCallback _writeCompleteCallback;
  bool _retry;   // atomic
  bool _connect; // atomic
  // always in loop thread
  int _nextConnId;
  Mutex _mutex;
  TcpConnectionPtr _connection; // @GuardedBy mutex_
};
}
}

#endif
