#ifndef WALLE_NET_CHANNEL_H_
#define WALLE_NET_CHANNEL_H_

#include <walle/algo/functional.h>
#include <walle/algo/memory.h>
#include <walle/sys/Time.h>

using namespace walle::sys;

namespace walle {
namespace net {
class EventLoop;

class Channel {
 public:
  typedef std::function<void()> EventCallback;
  typedef std::function<void(Time)> ReadEventCallback;
  Channel();
  Channel(EventLoop* loop, int sockfd);
  ~Channel();
  void setUp(EventLoop* loop, int sockfd)
  {_loop = loop;_fd = sockfd;}
  void handleEvent(Time receiveTime);
  void setReadCallback(const ReadEventCallback& cb)
  { _readCallback = cb; }
  void setWriteCallback(const EventCallback& cb)
  { _writeCallback = cb; }
  void setCloseCallback(const EventCallback& cb)
  { _closeCallback = cb; }
  void setErrorCallback(const EventCallback& cb)
  { _errorCallback = cb; }

  /// Tie this channel to the owner object managed by shared_ptr,
  /// prevent the owner object being destroyed in handleEvent.
  void tie(const std::shared_ptr<void>&);

  int fd() const { return _fd; }
  int events() const { return _events; }
  void set_revents(int revt) { _revents = revt; } // used by pollers
  // int revents() const { return revents_; }
  bool isNoneEvent() const { return _events == kNoneEvent; }

  void enableReading() { _events |= kReadEvent; update(); }
  // void disableReading() { events_ &= ~kReadEvent; update(); }
  void enableWriting() { _events |= kWriteEvent; update(); }
  void disableWriting() { _events &= ~kWriteEvent; update(); }
  void disableAll() { _events = kNoneEvent; update(); }
  bool isWriting() const { return _events & kWriteEvent; }
  void set_index(int idx) { _index = idx; }
  // for Poller
  int index() { return _index; }
  void index(int idx) { _index = idx; }

  // for debug
  string reventsToString() const;

  void doNotLogHup() { _logHup = false; }

  EventLoop* ownerLoop() { return _loop; }
  void remove();

 private:
  void update();
  void handleEventWithGuard(Time receiveTime);

  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;

  EventLoop*            _loop;
  int                   _fd;
  int                   _events;
  int                   _revents; // it's the received event types of epoll or poll
  int                   _index; // used by Poller.
  bool                  _logHup;

  std::weak_ptr<void> _tie;
  bool                  _tied;
  bool                  _eventHandling;
  bool                  _addedToLoop;
  ReadEventCallback     _readCallback;
  EventCallback         _writeCallback;
  EventCallback         _closeCallback;
  EventCallback         _errorCallback;
};

}
}

#endif
