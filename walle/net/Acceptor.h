#ifndef DYLIN_ACCEPTOR_H_
#define DYLIN_ACCEPTOR_H_
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include <walle/net/Channel.h>
#include <walle/net/Socket.h>

namespace walle {
namespace net {

class EventLoop;
class AddrInet;

class Acceptor : boost::noncopyable
{
 public:
  typedef boost::function<void (int sockfd,
                                 AddrInet&)> NewConnectionCallback;

  Acceptor(EventLoop* loop,  AddrInet& listenAddr, bool reuseport );
  ~Acceptor();

  void setNewConnectionCallback(const NewConnectionCallback& cb)
  { _newConnectionCallback = cb; }

  bool listenning() const { return _listenning; }
  void listen();

 private:
  void handleRead();

  EventLoop* _loop;
  Socket _acceptSocket;
  Channel _acceptChannel;
  NewConnectionCallback _newConnectionCallback;
  bool _listenning;
  int _idleFd;
};

}
}


#endif
