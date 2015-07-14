#include <walle/net/TimerTask.h>
#include <assert.h>

namespace walle{
namespace net{
	
AtomicInt64 TimerTask::gSeqnum;

TimerTask::TimerTask(const Time& expire, 
						int64_t inter, 
						void *user_data):
	_userData(user_data),
	_expiration(expire),
	_inerval(inter),
	_repeat(_inerval > 0 ?true:false),
	_sequence(gSeqnum.incrementAndGet())
{

}

TimerTask::~TimerTask()
{

}

void TimerTask::reset(const Time & t)
{
	if(_repeat) {
		_expiration = t + _inerval;
	} else {
		_expiration = t;	
	}
}

void TimerTask::restart()
{
	assert(_repeat);
	if(_repeat) {
		_expiration = Time::now() + _inerval;
	}
}

}
}
