#ifndef WALLE_TIMER_H_
#define WALLE_TIMER_H_

#include <set>
#include <vector>
#include <stdint.h>
#include <walle/sys/wallesys.h>
#include <walle/net/Callback.h>
#include <walle/net/Channel.h>
#include <walle/net/TimerId.h>
#include <walle/net/Timertask.h>
using namespace walle::sys;
namespace walle {
namespace net{

class EventLoop;
class TimerTask;
class TimerId;

///
/// A best efforts timer queue.
/// No guarantee that the callback will be on time.
///
class Timer
{
 public:
  Timer(EventLoop* loop);
  ~Timer();

  ///
  /// Schedules the callback to be run at given time,
  /// repeats if @c interval > 0.0.
  ///
  /// Must be thread safe. Usually be called from other threads.
  TimerId addTimer(const TimerCallback& cb,
                   Time when,
                   int64_t interval);

  void cancel(TimerId timerId);
private:
  Timer& operator=(const Timer&);
  Timer(const Timer&);
 private:

  // FIXME: use unique_ptr<Timer> instead of raw pointers.
  typedef std::pair<Time, TimerTask*> Entry;
  typedef std::set<Entry> TaskList;
  typedef std::pair<TimerTask*, int64_t> ActiveTask;
  typedef std::set<ActiveTask> ActiveTaskSet;

  void addTimerInLoop(TimerTask* task);
  void cancelInLoop(TimerId timerId);
  // called when timerfd alarms
  void handleRead();
  // move out all expired timers
  std::vector<Entry> getExpired(Time now);
  void reset(const std::vector<Entry>& expired, Time now);

  bool insert(TimerTask* task);

  EventLoop* _loop;
  const int _timerfd;
  Channel _timerfdChannel;
  // Timer list sorted by expiration
  TaskList _tasks;

  // for cancel()
  ActiveTaskSet _activeTasks;
  bool _callingExpiredTasks; /* atomic */
  ActiveTaskSet _cancelingTasks;
};

}
}
#endif