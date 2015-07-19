#ifndef DYLIN_CONNECTION_H_
#define DYLIN_CONNECTION_H_

#include <walle/sys/wallesys.h>
#include <walle/net/Callback.h>
#include <walle/net/Buffer.h>
#include <walle/net/Addrinet.h>

#include <boost/any.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

// struct tcp_info is in <netinet/tcp.h>
struct tcp_info;
using namespace walle::sys;
namespace walle {
namespace net {

class Channel;
class EventLoop;
class Socket;

///
/// TCP connection, for both client and server usage.
///
/// This is an interface class, so don't expose too much details.
class TcpConnection : boost::noncopyable,
                      public boost::enable_shared_from_this<TcpConnection>
{
 public:
  /// Constructs a TcpConnection with a connected sockfd
  ///
  /// User should not create this object.
  TcpConnection(EventLoop* loop,
                const string& name,
                int sockfd,
                const AddrInet& localAddr,
                const AddrInet& peerAddr);
  ~TcpConnection();

  EventLoop* getLoop() const { return _loop; }
  const string& name() const { return _name; }
  const AddrInet& localAddress() const { return _localAddr; }
  const AddrInet& peerAddress() const { return _peerAddr; }
  bool connected() const { return _state == kConnected; }
  // return true if success.
  bool getTcpInfo(struct tcp_info*) const;
  string getTcpInfoString() const;

  void send(const void* message, int len);
  void send(const StringPice& message);
  
  void send(Buffer* message);  // this one will swap data
  void shutdown();
  void forceClose();
  void forceCloseWithDelay(int64_t seconds);
  void setTcpNoDelay(bool on);

  void setContext(const boost::any& context)
  { _context = context; }

  const boost::any& getContext() const
  { return _context; }

  boost::any* getMutableContext()
  { return &_context; }

  void setConnectionCallback(const ConnectionCallback& cb)
  { _connectionCallback = cb; }

  void setMessageCallback(const MessageCallback& cb)
  { _messageCallback = cb; }

  void setWriteCompleteCallback(const WriteCompleteCallback& cb)
  { _writeCompleteCallback = cb; }

  void setHighWaterMarkCallback(const HighWaterMarkCallback& cb, size_t 
highWaterMark)
  { _highWaterMarkCallback = cb; _highWaterMark = highWaterMark; }

  /// Advanced interface
  Buffer* inputBuffer()
  { return &_inputBuffer; }

  Buffer* outputBuffer()
  { return &_outputBuffer; }

  /// Internal use only.
  void setCloseCallback(const CloseCallback& cb)
  { _closeCallback = cb; }

  // called when TcpServer accepts a new connection
  void connectEstablished();   // should be called only once
  // called when TcpServer has removed me from its map
  void connectDestroyed();  // should be called only once

 private:
  enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };
  void handleRead(Time receiveTime);
  void handleWrite();
  void handleClose();
  void handleError();
  // void sendInLoop(string&& message);
  void sendInLoop(const StringPice& message);
  void sendInLoop(const void* message, size_t len);
  void shutdownInLoop();
  // void shutdownAndForceCloseInLoop(double seconds);
  void forceCloseInLoop();
  void setState(StateE s) { _state = s; }

  EventLoop*                 _loop;
  const string               _name;
  StateE                     _state; 
  boost::scoped_ptr<Socket>  _socket;
  boost::scoped_ptr<Channel> _channel;
  ConnectionCallback         _connectionCallback;
  MessageCallback            _messageCallback;
  WriteCompleteCallback      _writeCompleteCallback;
  HighWaterMarkCallback      _highWaterMarkCallback;
  CloseCallback              _closeCallback;
  Buffer                     _inputBuffer;
  Buffer                     _outputBuffer;
  boost::any                 _context;
  AddrInet                   _localAddr;
  AddrInet                   _peerAddr;
  size_t                     _highWaterMark;
};

typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;


}
}
#endif
