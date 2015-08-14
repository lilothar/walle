#include <walle/sys/Logging.h>
#include <walle/net/Channel.h>
#include <walle/net/Eventloop.h>

#include <sstream>

#include <poll.h>

using namespace walle::net;

using namespace walle::sys;


const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel()
  : _loop(NULL),
    _fd(-1),
    _events(0),
    _revents(0),
    _index(-1),
    _logHup(true),
    _tied(false),
    _eventHandling(false),
    _addedToLoop(false)
{
}

Channel::Channel(EventLoop* loop, int sockfd)
  : _loop(loop),
    _fd(sockfd),
    _events(0),
    _revents(0),
    _index(-1),
    _logHup(true),
    _tied(false),
    _eventHandling(false),
    _addedToLoop(false)
{
}

Channel::~Channel()
{
  assert(!_eventHandling);
  assert(!_addedToLoop);
  if (_loop->isInLoopThread())
  {
    assert(!_loop->hasChannel(this));
  }
}

void Channel::tie(const boost::shared_ptr<void>& obj)
{
  _tie = obj;
  _tied = true;
}

void Channel::update()
{
  _addedToLoop = true;
  _loop->updateChannel(this);
}

void Channel::remove()
{
  assert(isNoneEvent());
  _addedToLoop = false;
  _loop->removeChannel(this);
}

void Channel::handleEvent(Time receiveTime)
{

  boost::shared_ptr<void> guard;
  if (_tied)
  {
    guard = _tie.lock();
    if (guard)
    {
      handleEventWithGuard(receiveTime);
    }
  }
  else
  {
    handleEventWithGuard(receiveTime);
  }
}

void Channel::handleEventWithGuard(Time receiveTime)
{
  _eventHandling = true;
  if ((_revents & POLLHUP) && !(_revents & POLLIN))
  {
    if (_logHup)
    {
      LOG_WARN<<"Channel::handle_event() POLLHUP";
    }
    if (_closeCallback) _closeCallback();
  }

  if (_revents & POLLNVAL)
  {
    LOG_WARN<<"Channel::handle_event() POLLNVAL";
  }

  if (_revents& (POLLERR | POLLNVAL))
  {
    if (_errorCallback) _errorCallback();
  }
  if (_revents & (POLLIN | POLLPRI | POLLRDHUP))
  {
    if (_readCallback) _readCallback(receiveTime);
  }
  if (_revents & POLLOUT)
  {
    if (_writeCallback) _writeCallback();
  }
  _eventHandling = false;
}

string Channel::reventsToString() const
{
  std::ostringstream oss;
  oss << _fd << ": ";
  if (_revents & POLLIN)
    oss << "IN ";
  if (_revents & POLLPRI)
    oss << "PRI ";
  if (_revents & POLLOUT)
    oss << "OUT ";
  if (_revents & POLLHUP)
    oss << "HUP ";
  if (_revents & POLLRDHUP)
    oss << "RDHUP ";
  if (_revents & POLLERR)
    oss << "ERR ";
  if (_revents & POLLNVAL)
    oss << "NVAL ";

  return oss.str().c_str();
}

