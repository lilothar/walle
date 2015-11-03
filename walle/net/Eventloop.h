#ifndef WALLE_NET_EVENTLOOP_H_
#define WALLE_NET_EVENTLOOP_H_
#include <vector>

#include <walle/algo/functional.h>
#include <walle/algo/noncopyable.h>
#include <walle/algo/scoped_ptr.h>

#include <walle/sys/wallesys.h>
#include <walle/net/Callback.h>
#include <walle/net/TimerId.h>
#include <stdint.h>

using namespace walle::sys;
namespace walle {
namespace net {
class Channel;
class Poller;
class Timer;

///
/// Reactor, at most one per thread.
///
/// This is an interface class, so don't expose too much details.
class EventLoop
{
 public:
  typedef std::function<void()> Functor;

  EventLoop();
  ~EventLoop();  // force out-line dtor, for scoped_ptr members.

  ///
  /// Loops forever.
  ///
  /// Must be called in the same thread as creation of the object.
  ///
  void loop();

  /// Quits loop.
  ///
  /// This is not 100% thread safe, if you call through a raw pointer,
  /// better to call through shared_ptr<EventLoop> for 100% safety.
  void quit();

  ///
  /// Time when poll returns, usually means data arrival.
  ///
  Time pollReturnTime() const { return _pollReturnTime; }

  int64_t iteration() const { return _iteration; }

  /// Runs callback immediately in the loop thread.
  /// It wakes up the loop, and run the cb.
  /// If in the same loop thread, cb is run within the function.
  /// Safe to call from other threads.
  void runInLoop(const Functor& cb);
  /// Queues callback in the loop thread.
  /// Runs after finish pooling.
  /// Safe to call from other threads.
  void queueInLoop(const Functor& cb);

  // timers

  ///
  /// Runs callback at 'time'.
  /// Safe to call from other threads.
  ///
  TimerId runAt(const Time& time, const TimerCallback& cb);
  ///
  /// Runs callback after @c delay seconds.
  /// Safe to call from other threads.
  ///
  TimerId runAfter(int64_t delay, const TimerCallback& cb);
  ///
  /// Runs callback every @c interval seconds.
  /// Safe to call from other threads.
  ///
  TimerId runEvery(int64_t interval, const TimerCallback& cb);
  ///
  /// Cancels the timer.
  /// Safe to call from other threads.
  ///
  void cancel(TimerId timerId);

  // internal usage
  void wakeup();
  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);
  bool hasChannel(Channel* channel);

  // pid_t threadId() const { return threadId_; }
  void assertInLoopThread()
  {
    if (!isInLoopThread())
    {
      abortNotInLoopThread();
    }
  }
  bool isInLoopThread() const { return _threadId == walle::LocalThread::tid(); }
  // bool callingPendingFunctors() const { return callingPendingFunctors_; }
  bool eventHandling() const { return _eventHandling; }

  static EventLoop* getEventLoopOfCurrentThread();

 private:
  void abortNotInLoopThread();
  void handleRead();  // waked up
  void doPendingFunctors();

  void printActiveChannels() const; // DEBUG

  typedef std::vector<Channel*> ChannelList;

  bool                      _looping; /* atomic */
  bool                      _quit; /* atomic and shared between threads, okay on x86, I guess. */
  bool                      _eventHandling; /* atomic */
  bool                      _callingPendingFunctors; /* atomic */
  int64_t                   _iteration;
  const pthread_t           _threadId;
  Time                      _pollReturnTime;
  std::scoped_ptr<Poller>   _poller;
  std::scoped_ptr<Timer>    _timer;
  int                       _wakeupFd;
  // unlike in TimerQueue, which is an internal class,
  // we don't expose Channel to client.
  std::scoped_ptr<Channel>  _wakeupChannel;
  ChannelList                _activeChannels;
  Channel                   *_currentActiveChannel;
  Mutex                      _mutex;
  std::vector<Functor>       _pendingFunctors; // @GuardedBy mutex_
};
}

}
#endif