#include <walle/net/TcpConnection.h>
#include <walle/net/Channel.h>
#include <walle/net/Eventloop.h>
#include <walle/net/Socket.h>
#include <walle/sys/Weakcall.h>
#include <walle/algo/functional.h>

#include <errno.h>

using namespace walle::sys;
namespace walle{
namespace net  {

void defaultConnectionCallback(const TcpConnectionPtr& conn)
{
  	LOG_TRACE<<conn->localAddress().toString()
        <<" -> "<<conn->peerAddress().toString()
        <<(conn->connected() ? " UP" : " DOWN");
}

void defaultMessageCallback(const TcpConnectionPtr& conn,
                            Buffer* buffer,
                            Time receiveTime)

{
	(void)conn;
	(void)receiveTime;
  	buffer->retrieveAll();
}
typedef std::function<void (const StringPice&)> connSendFunc;
struct ConnSendAdapter{
	explicit ConnSendAdapter(const connSendFunc& func, 
								const StringPice&pice)
		:_func(func),
		 _data(pice.data(),pice.size())
	{
		assert(_func);
	}

	void doSend()
	{
		_func(StringPice(_data.c_str(),_data.size()));
	}
	connSendFunc _func;
	string _data;
	

}; 
TcpConnection::TcpConnection(EventLoop* loop,
                             const string& nameArg,
                             int sockfd,
                             const AddrInet& localAddr,
                             const AddrInet& peerAddr)
  : 
  	_lastactive(Time::now()),
  	_timerOutSec(0),
  	_loop(loop),
    _name(nameArg),
    _state(kConnecting),
    _socket(new Socket(true)),
    _channel(new Channel(loop, sockfd)),
    _localAddr(localAddr),
    _peerAddr(peerAddr),
    _highWaterMark(64*1024*1024)
{
	_socket->setFd(sockfd);
	_channel->setReadCallback(
	std::bind(&TcpConnection::handleRead, this, _1));
	_channel->setWriteCallback(
	std::bind(&TcpConnection::handleWrite, this));
	_channel->setCloseCallback(
	std::bind(&TcpConnection::handleClose, this));
	_channel->setErrorCallback(
	std::bind(&TcpConnection::handleError, this));
	
	LOG_DEBUG<<"TcpConnection::connect[ "<<_name<<" ] at fd="<<sockfd;
	_socket->setKeepAlive(true);
}

TcpConnection::~TcpConnection()
{
  LOG_DEBUG<<"TcpConnection::disconnect[ "<<_name<<" ] at fd="<<_channel->fd();
  assert(_state == kDisconnected);
  
}

void TcpConnection::enableTimerOut(int64_t secs) 
{
	if(_timerWheelHandler.valid()) {
		_timerWheelHandler.release();
	}
	_timerWheelHandler = _loop->runEverySec(secs,std::bind(&TcpConnection::onTimerOut,this));
}

void TcpConnection::disableTimerOut() 
{
	_timerWheelHandler.release();
}

void TcpConnection::onTimerOut()
{

	Time now = Time::now();
	Time diff = now - _lastactive;
	if(diff.toSeconds() >= _timerOutSec) {
		forceCloseInLoop();
	}
}


bool TcpConnection::getTcpInfo(struct tcp_info* tcpi) const
{
  return _socket->getTcpInfo(tcpi);
}

string TcpConnection::getTcpInfoString() const
{
  char buf[1024];
  buf[0] = '\0';
  _socket->getTcpInfoString(buf, sizeof buf);
  return buf;
}

void TcpConnection::send(const void* data, int len)
{
  send(StringPice(static_cast<const char*>(data), len));
}

void TcpConnection::send(const StringPice& message)
{
  if (_state == kConnected)
  {
    if (_loop->isInLoopThread())
    {
      sendInLoop(message);
    }
    else
    {
    	connSendFunc cb(std::bind(&TcpConnection::sendPiceInLoop, this,message));
    	ConnSendAdapter ad(cb, message);
      _loop->runInLoop(
          std::bind(&ConnSendAdapter::doSend,ad));
    }
  }
}


void TcpConnection::send(Buffer* buf)
{
	LOG_DEBUG<<"TcpConnection::send";
  if (_state == kConnected)
  {
    if (_loop->isInLoopThread())
    {
      sendInLoop(buf->peek(), buf->readableBytes());
      buf->retrieveAll();
    }
    else
    {
      StringPice message(buf->peek(),buf->readableBytes()); 
	  connSendFunc cb = std::bind(&TcpConnection::sendPiceInLoop,this,std::_1);
      ConnSendAdapter ad(cb, message);
	  buf->retrieveAll();
      _loop->runInLoop(
          std::bind(&ConnSendAdapter::doSend,ad));
    }
  }
}

void TcpConnection::sendInLoop(const StringPice& message)
{
	LOG_DEBUG<<"TcpConnection::sendInLoop";
  sendInLoop(message.data(), message.size());
}

void TcpConnection::sendPiceInLoop(const StringPice& message)
{
	LOG_DEBUG<<"TcpConnection::sendPiceInLoop";
  sendInLoop(message.data(), message.size());
}

void TcpConnection::sendInLoop(const void* data, size_t len)
{
	LOG_DEBUG<<"TcpConnection::sendInLoop";
  _loop->assertInLoopThread();
  ssize_t nwrote = 0;
  size_t remaining = len;
  bool faultError = false;
  if (_state == kDisconnected)
  {
    LOG_WARN<<"disconnected, give up writing";
    return;
  }
  	_lastactive = Time::now();
  // if no thing in output queue, try writing directly
  if (!_channel->isWriting() && _outputBuffer.readableBytes() == 0)
  {
    nwrote = ::write(_channel->fd(), data, len);
    if (nwrote >= 0)
    {
      remaining = len - nwrote;
      if (remaining == 0 && _writeCompleteCallback)
      {
        _loop->queueInLoop(std::bind(_writeCompleteCallback,shared_from_this()));
      }
    }
    else // nwrote < 0
    {
      nwrote = 0;
      if (errno != EWOULDBLOCK)
      {
        LOG_ERROR<<"TcpConnection::sendInLoop error "<< errno;
        if (errno == EPIPE || errno == ECONNRESET) // FIXME: any others?
        {
          faultError = true;
        }
      }
    }
  }

  assert(remaining <= len);
  if (!faultError && remaining > 0) {
    size_t oldLen = _outputBuffer.readableBytes();
    if (oldLen + remaining >= _highWaterMark
        && oldLen < _highWaterMark
        && _highWaterMarkCallback) {
      _loop->queueInLoop(std::bind(_highWaterMarkCallback, 
	  	shared_from_this(), oldLen + remaining));
    }
    _outputBuffer.append(static_cast<const char*>(data)+nwrote, remaining);
    if (!_channel->isWriting())
    {
      _channel->enableWriting();
    }
  }
}

void TcpConnection::shutdown()
{
	LOG_DEBUG<<"TcpConnection::shutdown";
  // FIXME: use compare and swap
  if (_state == kConnected)
  {
    setState(kDisconnecting);
    // FIXME: shared_from_this()?
    _loop->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
	_loop->wakeup();
  }
}

void TcpConnection::shutdownInLoop()
{
	LOG_DEBUG<<"TcpConnection::shutdownInLoop";
  _loop->assertInLoopThread();
  if (!_channel->isWriting())
  {
    // we are not writing
    _socket->shutdown();
  }
}

void TcpConnection::forceClose()
{
  // FIXME: use compare and swap
  LOG_DEBUG<<"TcpConnection::forceClose";
  if (_state == kConnected || _state == kDisconnecting)
  {
    setState(kDisconnecting);
    _loop->queueInLoop(std::bind(&TcpConnection::forceCloseInLoop, 
			shared_from_this()));
  }
}

void TcpConnection::forceCloseWithDelay(int64_t seconds)
{
	LOG_DEBUG<<"TcpConnection::forceCloseWithDelay";
  if (_state == kConnected || _state == kDisconnecting)
  {
    setState(kDisconnecting);

    _loop->runAfter(
        seconds*1000000,
        makeWeakCallback(shared_from_this(),
                         &TcpConnection::forceClose)); 
  }
}

void TcpConnection::forceCloseInLoop()
{
	LOG_DEBUG<<"TcpConnection::forceCloseInLoop";

  _loop->assertInLoopThread();
  if (_state == kConnected || _state == kDisconnecting)
  {
    // as if we received 0 byte in handleRead();
    _socket->shutdown();
    handleClose();
  }
}

void TcpConnection::setTcpNoDelay(bool on)
{
  _socket->setTcpNoDelay(on);
}

void TcpConnection::connectEstablished()
{
	LOG_DEBUG<<"TcpConnection::connectEstablished";
  _loop->assertInLoopThread();
  assert(_state == kConnecting);
  setState(kConnected);
  _channel->tie(shared_from_this());
  _channel->enableReading();

  _connectionCallback(shared_from_this());
}

void TcpConnection::connectDestroyed()
{
		
  _loop->assertInLoopThread();
  if (_state == kConnected)
  {
    setState(kDisconnected);
	_socket->shutdown();
    _channel->disableAll();

    _connectionCallback(shared_from_this());
  }
  _channel->remove();
  LOG_DEBUG<<"TcpConnection::connectDestroyed";
}

void TcpConnection::handleRead(Time receiveTime)
{
	LOG_DEBUG<<"TcpConnection::handleRead";

  _loop->assertInLoopThread();
  int savedErrno = 0;
  	_lastactive = Time::now();
  ssize_t n = _inputBuffer.readFd(_channel->fd(), &savedErrno);
  if (n > 0)
  {
    _messageCallback(shared_from_this(), &_inputBuffer, receiveTime);
  }
  else if (n == 0)
  {
    handleClose();
  }
  else
  {
    errno = savedErrno;
    LOG_ERROR<<"TcpConnection::handleRead";
    handleError();
  }
   
}

void TcpConnection::handleWrite()
{
	LOG_DEBUG<<"TcpConnection::handleWrite";
  _loop->assertInLoopThread();
  if (_channel->isWriting())
  {
  	_lastactive = Time::now();
    ssize_t n = ::write(_channel->fd(),
                               _outputBuffer.peek(),
                               _outputBuffer.readableBytes());
    if (n > 0) {
      _outputBuffer.retrieve(n);
      if (_outputBuffer.readableBytes() == 0) {
	  	_channel->disableWriting();
        if (_writeCompleteCallback) {
			_loop->queueInLoop(std::bind(_writeCompleteCallback, 
										shared_from_this()));
        }
       	if (_state == kDisconnecting) {
         		 shutdownInLoop();
        }
      }
    } else {
    	if(errno != EAGAIN ) 
			handleClose();
       
    }
  }
  else {
    LOG_TRACE<<"Connection fd = "<<_channel->fd() <<"is down, no more writing";
  }
}

void TcpConnection::handleClose()
{
	LOG_DEBUG<<"TcpConnection::handleClose";
  _loop->assertInLoopThread();
  LOG_TRACE<<"fd = "<<  _channel->fd()<<" state ="<< (int)_state;
  assert(_state == kConnected || _state == kDisconnecting);
  // we don't close fd, leave it to dtor, so we can find leaks easily.
  setState(kDisconnected);
  _channel->disableAll();

  TcpConnectionPtr guardThis(shared_from_this());
  _connectionCallback(guardThis);
  // must be the last line
  _closeCallback(guardThis);
}

void TcpConnection::handleError()
{
  int err = _socket->getSoError();
  LOG_ERROR<<"TcpConnection::handleError [ "<<_name<<" ] - SO_ERROR ="<< err;
}
}
}
