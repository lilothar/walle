#ifndef WALLE_NET_CONNECTION_H_
#define WALLE_NET_CONNECTION_H_

#include <walle/sys/wallesys.h>
#include <walle/net/Callback.h>
#include <walle/net/Buffer.h>
#include <walle/net/Addrinet.h>

#include <walle/algo/any.h>
#include <walle/algo/noncopyable.h>
#include <walle/algo/memory.h>


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
class TcpConnection : std::noncopyable,
                      public std::enable_shared_from_this<TcpConnection>
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

  void setContext(const std::any& context)
  { _context = context; }

  const std::any& getContext() const
  { return _context; }

  std::any* getMutableContext()
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
  void sendPiceInLoop(const StringPice& message);
  void shutdownInLoop();
  // void shutdownAndForceCloseInLoop(double seconds);
  void forceCloseInLoop();
  void setState(StateE s) { _state = s; }

  EventLoop*                 _loop;
  const string               _name;
  StateE                     _state; 
  std::scoped_ptr<Socket>    _socket;
  std::scoped_ptr<Channel>   _channel;
  ConnectionCallback         _connectionCallback;
  MessageCallback            _messageCallback;
  WriteCompleteCallback      _writeCompleteCallback;
  HighWaterMarkCallback      _highWaterMarkCallback;
  CloseCallback              _closeCallback;
  Buffer                     _inputBuffer;
  Buffer                     _outputBuffer;
  std::any                   _context;
  AddrInet                   _localAddr;
  AddrInet                   _peerAddr;
  size_t                     _highWaterMark;
};

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;


}
}
#endif
