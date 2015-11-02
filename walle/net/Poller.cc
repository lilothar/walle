#include <walle/net/Poller.h>

#include <walle/net/Channel.h>

#include <walle/sys/Logging.h>
#include <walle/net/Channel.h>

#include <assert.h>
#include <errno.h>
#include <poll.h>
#include <sys/epoll.h>


namespace
{
const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;
}

using namespace walle::sys;
namespace walle {
namespace net {
Poller::Poller(EventLoop* loop):_ownerLoop(loop),
	_epollfd(::epoll_create1(EPOLL_CLOEXEC)),
	_events(kInitEventListSize)
{
	if(_epollfd < 0) {
		LOG_ERROR<<"create epoll fail";
	}
}

Poller::~Poller()
{
	::close(_epollfd);

}
Time Poller::poll(int timeoutMs, ChannelList* activeChannels)
{
  int numEvents = ::epoll_wait(_epollfd,
                               &*_events.begin(),
                               static_cast<int>(_events.size()),
                               timeoutMs);
  int savedErrno = errno;
  Time now(Time::now());
  if (numEvents > 0) {
    fillActiveChannels(numEvents, activeChannels);
    if (numEvents == static_cast<int>(_events.size())) {
      _events.resize(_events.size()*2);
    }
  } else if (numEvents < 0) {
    if (savedErrno != EINTR)
    {
      errno = savedErrno;
      LOG_ERROR<<"EPollPoller::poll() errno: "<<savedErrno;
    }
  }
  return now;
}

void Poller::fillActiveChannels(int numEvents,
                                     ChannelList* activeChannels) const
{
  assert(numEvents <= (int)_events.size());
  for (int i = 0; i < numEvents; ++i)
  {
    Channel* channel = static_cast<Channel*>(_events[i].data.ptr);
    channel->set_revents(_events[i].events);
    activeChannels->push_back(channel);
  }
}

void Poller::updateChannel(Channel* channel)
{
  Poller::assertInLoopThread();
  const int index = channel->index();
  if (index == kNew || index == kDeleted)
  {
    // a new one, add with EPOLL_CTL_ADD
    int fd = channel->fd();
    if (index == kNew)
    {
      //assert(_channels.find(fd) == _channels.end());
      _channels[fd] = channel;
    }
    else // index == kDeleted
    {
      //assert(_channels.find(fd) != _channels.end());
      //assert(_channels[fd] == channel);
    }
    channel->set_index(kAdded);
    update(EPOLL_CTL_ADD, channel);
  }
  else {
    // update existing one with EPOLL_CTL_MOD/DEL
    int fd = channel->fd();
    (void)fd;
    //assert(_channels.find(fd) != _channels.end());
    //assert(_channels[fd] == channel);
    //assert(index == kAdded);
    if (channel->isNoneEvent()){
			update(EPOLL_CTL_DEL, channel);
      channel->set_index(kDeleted);
    } else {
      update(EPOLL_CTL_MOD, channel);
    }
  }
}
void Poller::removeChannel(Channel* channel)
{
  Poller::assertInLoopThread();
  int fd = channel->fd();
  //assert(_channels.find(fd) != _channels.end());
  //assert(_channels[fd] == channel);
  //assert(channel->isNoneEvent());
  int index = channel->index();
 // assert(index == kAdded || index == kDeleted);
  size_t n = _channels.erase(fd);
  (void)n;
  //assert(n == 1);

  if (index == kAdded)
  {
    update(EPOLL_CTL_DEL, channel);
  }
  channel->set_index(kNew);
}

void Poller::update(int operation, Channel* channel)
{
  struct epoll_event event;
  bzero(&event, sizeof event);
  event.events = channel->events();
  event.data.ptr = channel;
  int fd = channel->fd();
  if (::epoll_ctl(_epollfd, operation, fd, &event) < 0) {
    if (operation == EPOLL_CTL_DEL) {
		LOG_ERROR<<"epoll ctl "<<operation<<" error";
	}
     
  }
}

bool Poller::hasChannel(Channel* channel) const
{
  assertInLoopThread();
  ChannelMap::const_iterator it = _channels.find(channel->fd());
  return it != _channels.end() && it->second == channel;
}
}
}
