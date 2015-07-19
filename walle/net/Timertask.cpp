#include <walle/net/Timertask.h>
using namespace walle::sys;
namespace walle {
namespace net {
	
AtomicInt64 TimerTask::_gTimerNum;

void TimerTask::restart(const Time &now)
{
  if (_repeat)
  {
    _expiration = now + _interval;
  }
  else
  {
    _expiration = Time();
  }
}
}

}
