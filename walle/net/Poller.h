#ifndef WALLE_POLLER_H_
#define WALLE_POLLER_H_

#include <tr1/unordered_map>
#include <vector>
#include <walle/sys/Time.h>
#include <walle/net/Eventloop.h>

using namespace walle::sys;

struct epoll_event;

namespace walle {
namespace net{

class Channel;

class Poller
{
 public:
  typedef std::vector<Channel*> ChannelList;

  Poller(EventLoop* loop);
  virtual ~Poller();

  /// Polls the I/O events.
  /// Must be called in the loop thread.
  Time poll(int timeoutMs, ChannelList* activeChannels);

  /// Changes the interested I/O events.
  /// Must be called in the loop thread.
  void updateChannel(Channel* channel);

  /// Remove the channel, when it destructs.
  /// Must be called in the loop thread.
  void removeChannel(Channel* channel);

  bool hasChannel(Channel* channel) const;

  void assertInLoopThread() const
  {
    _ownerLoop->assertInLoopThread();
  }

 protected:
  typedef std::tr1::unordered_map<int, Channel*> ChannelMap;
  ChannelMap _channels;

 private:
  EventLoop* _ownerLoop;
  static const int kInitEventListSize = 16;

  void fillActiveChannels(int numEvents,
                          ChannelList* activeChannels) const;
  void update(int operation, Channel* channel);

  typedef std::vector<struct epoll_event> EventList;

  int _epollfd;
  EventList _events;
};

}
}
#endif
