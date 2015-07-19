#ifndef DYLIN_CONNECTOR_H_
#define DYLIN_CONNECTOR_H_

#include <walle/net/Addrinet.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <walle/net/Socket.h>
namespace walle {
namespace net {

class Channel;
class EventLoop;

class Connector : boost::noncopyable,
                  public boost::enable_shared_from_this<Connector>
{
 public:
  typedef boost::function<void (int sockfd)> NewConnectionCallback;

  Connector(EventLoop* loop, const AddrInet& serverAddr);
  ~Connector();

  void setNewConnectionCallback(const NewConnectionCallback& cb)
  { _newConnectionCallback = cb; }

  void start();  // can be called in any thread
  void restart();  // must be called in loop thread
  void stop();  // can be called in any thread

  const AddrInet& serverAddress() const { return _serverAddr; }

 private:
  enum States { kDisconnected, kConnecting, kConnected };
  static const int kMaxRetryDelayMs = 30*1000;
  static const int kInitRetryDelayMs = 500;

  void setState(States s) { _state = s; }
  void startInLoop();
  void stopInLoop();
  void connect();
  void connecting(int sockfd);
  void handleWrite();
  void handleError();
  void retry(int sockfd);
  int removeAndResetChannel();
  void resetChannel();
  Socket                     _socket;
  EventLoop*                 _loop;
  AddrInet                   _serverAddr;
  bool                       _connect; // atomic
  States                     _state;  // FIXME: use atomic variable
  boost::scoped_ptr<Channel> _channel;
  NewConnectionCallback      _newConnectionCallback;
  int                        _retryDelayMs;
};

}
}
#endif
