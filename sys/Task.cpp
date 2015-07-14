#include <walle/sys/wallesys.h>

namespace walle {
namespace sys {

AtomicInt64 ITask::gSeqnum;

Task::Task(Taskfun factor, void *args,int32_t priority):
_args(args),
_factor(factor),
_seq(gSeqnum.getAndSet()),
_priority(priority),

_canceled(false),

{

}

void Task::runTask()
{
	if(_canceled) {
		return;
	}
	if(_factor) {
		_factor(_args, this);
	}
}

}

}
