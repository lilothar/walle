#include <walle/net/Connector.h>
#include <walle/sys/wallesys.h>
#include <walle/net/Channel.h>
#include <walle/net/Eventloop.h>
#include <walle/net/Socket.h>
#include <boost/bind.hpp>

#include <errno.h>

using namespace walle::sys;
using namespace walle::net;

const int Connector::kMaxRetryDelayMs;

Connector::Connector(EventLoop* loop, const AddrInet& serverAddr)
  :_socket(),
  _loop(loop),
   _serverAddr(serverAddr),
    _connect(false),
    _state(kDisconnected),
    
    _retryDelayMs(kInitRetryDelayMs)
{
	LOG_DEBUG<<"connector creatr "<<this;
}

Connector::~Connector()
{
  assert(!_channel);
  LOG_DEBUG<<"connector destory "<<this;
}

void Connector::start()
{
  _connect = true;
  _loop->runInLoop(boost::bind(&Connector::startInLoop, this));
  LOG_DEBUG<<"connector start "<<this;
}

void Connector::startInLoop()
{
	LOG_DEBUG<<"connector startinloop "<<this;

  _loop->assertInLoopThread();
  assert(_state == kDisconnected);
  if (_connect)
  {
    connect();
  }
}

void Connector::stop()
{
	LOG_DEBUG<<"connector stop "<<this;
  _connect = false;
  _loop->queueInLoop(boost::bind(&Connector::stopInLoop, this)); 
}

void Connector::stopInLoop()
{
	LOG_DEBUG<<"connector stopinloop "<<this;
  _loop->assertInLoopThread();
  if (_state == kConnecting)
  {
    setState(kDisconnected);
    int sockfd = removeAndResetChannel();
    retry(sockfd);
  }
}

void Connector::connect()
{
 LOG_DEBUG<<"connector connect "<<this;
  int sockfd;
  bool ret = _socket.connect(_serverAddr);
  sockfd  = _socket.getFd();
  int savedErrno = (ret == true) ? 0 : errno;
  switch (savedErrno)
  {
    case 0:
    case EINPROGRESS:
    case EINTR:
    case EISCONN:
      connecting(sockfd);
      break;

    case EAGAIN:
    case EADDRINUSE:
    case EADDRNOTAVAIL:
    case ECONNREFUSED:
    case ENETUNREACH:
      retry(sockfd);
      break;

    case EACCES:
    case EPERM:
    case EAFNOSUPPORT:
    case EALREADY:
    case EBADF:
    case EFAULT:
    case ENOTSOCK:
      LOG_ERROR<<"connect error in Connector::startInLoop "<<savedErrno;
      _socket.close();
      break;

    default:
      LOG_ERROR<<"Unexpected error in Connector::startInLoop "<< savedErrno;
      _socket.close();
      break;
  }
}

void Connector::restart()
{
	LOG_DEBUG<<"connector restart "<<this;	
  _loop->assertInLoopThread();
  setState(kDisconnected);
  _retryDelayMs = kInitRetryDelayMs;
  _connect = true;
  startInLoop();
}

void Connector::connecting(int sockfd)
{
	LOG_DEBUG<<"connector connecting "<<this;	

  setState(kConnecting);
  assert(!_channel);
  _channel.reset(new Channel(_loop, sockfd));
  _channel->setWriteCallback(
      boost::bind(&Connector::handleWrite, this)); // FIXME: unsafe
  _channel->setErrorCallback(
      boost::bind(&Connector::handleError, this)); // FIXME: unsafe
  _channel->enableWriting();
}

int Connector::removeAndResetChannel()
{
	LOG_DEBUG<<"connector removeAndResetChannel "<<this;

  _channel->disableAll();
  _channel->remove();
  int sockfd = _channel->fd();
  _loop->queueInLoop(boost::bind(&Connector::resetChannel, this));
  return sockfd;
}

void Connector::resetChannel()
{
	LOG_DEBUG<<"connector resetChannel "<<this;
  _channel.reset();
}

void Connector::handleWrite()
{
	LOG_DEBUG<<"connector handleWrite "<<this;

  if (_state == kConnecting)
  {
    int sockfd = removeAndResetChannel();
	_socket.setFd(sockfd);
    int err = _socket.getSoError();
    if (err)
    {
      retry(sockfd);
    }
    else
    {
      setState(kConnected);
      if (_connect)
      {
        _newConnectionCallback(sockfd);
      }
      else
      {
      	_socket.setFd(sockfd);
        _socket.close();
      }
    }
  }
  else
  {
    // what happened?
    assert(_state == kDisconnected);
  }
}

void Connector::handleError()
{
	LOG_DEBUG<<"connector handleError "<<this;
  if (_state == kConnecting)
  {
    int sockfd = removeAndResetChannel();
	_socket.setFd(sockfd);
    int err = _socket.getSoError();
    LOG_ERROR<<"connect error SO_ERROR "<<err;
    retry(sockfd);
  }
}

void Connector::retry(int sockfd)
{
	LOG_DEBUG<<"connector retry "<<this;

	_socket.setFd(sockfd);

  	_socket.close();
 	 setState(kDisconnected);
 	 if (_connect)
	  {
	    _loop->runAfter(_retryDelayMs*1000,
                    boost::bind(&Connector::startInLoop, shared_from_this()));
 	   _retryDelayMs = std::min(_retryDelayMs * 2, kMaxRetryDelayMs);
 	 }
}
