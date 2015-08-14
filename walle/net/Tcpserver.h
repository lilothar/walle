#ifndef WALLE_TCPSERVER_H_
#define WALLE_TCPSERVER_H_

#include <walle/sys/Atomic.h>
#include <walle/net/TcpConnection.h>
#include <boost/function.hpp>
#include <tr1/unordered_map>

#include <map>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <walle/net/Addrinet.h>
#include <string>
#include <walle/net/Callback.h>
using namespace std;
using namespace walle::sys;
namespace walle {
namespace net {
class Acceptor;
class EventLoop;
class EventLoopThreadPool;

///
/// TCP server, supports single-threaded and thread-pool models.
///
/// This is an interface class, so don't expose too much details.
class TcpServer : boost::noncopyable
{
 public:
  typedef boost::function<void(EventLoop*)> ThreadInitCallback;
  enum Option
  {
    kNoReusePort,
    kReusePort,
  };

  //TcpServer(EventLoop* loop, const InetAddress& listenAddr);
  TcpServer(EventLoop* loop,
            AddrInet& listenAddr,
            const string& nameArg,
            Option option = kReusePort);
  ~TcpServer();  // force out-line dtor, for scoped_ptr members.

  const string& hostport() const { return _hostport; }
  const string& name() const { return _name; }
  EventLoop* getLoop() const { return _loop; }

  /// Set the number of threads for handling input.
  ///
  /// Always accepts new connection in loop's thread.
  /// Must be called before @c start
  /// @param numThreads
  /// - 0 means all I/O in loop's thread, no thread will created.
  ///   this is the default value.
  /// - 1 means all I/O in another thread.
  /// - N means a thread pool with N threads, new connections
  ///   are assigned on a round-robin basis.
  void setThreadNum(int numThreads);
  void setThreadInitCallback(const ThreadInitCallback& cb)
  { _threadInitCallback = cb; }
  /// valid after calling start()
  boost::shared_ptr<EventLoopThreadPool> threadPool()
  { return _threadPool; }

  /// Starts the server if it's not listenning.
  ///
  /// It's harmless to call it multiple times.
  /// Thread safe.
  void start();

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
  void newConnection(int sockfd, const AddrInet& peerAddr);
  /// Thread safe.
  void removeConnection(const TcpConnectionPtr& conn);
  /// Not thread safe, but in loop
  void removeConnectionInLoop(const TcpConnectionPtr& conn);

  typedef std::tr1::unordered_map<string, TcpConnectionPtr> ConnectionMap;
  EventLoop*                             _loop;  // the acceptor loop
  const string                           _hostport;
  const string                           _name;
  boost::scoped_ptr<Acceptor>            _acceptor; // avoid revealing Acceptor
  boost::shared_ptr<EventLoopThreadPool> _threadPool;
  ConnectionCallback                     _connectionCallback;
  MessageCallback                        _messageCallback;
  WriteCompleteCallback                  _writeCompleteCallback;
  ThreadInitCallback                     _threadInitCallback;
  AtomicInt32                            _started;
  // always in loop thread
  int                                    _nextConnId;
  ConnectionMap                          _connections;
};


}
}

#endif
