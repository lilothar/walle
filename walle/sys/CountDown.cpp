#include <walle/sys/CountDown.h>
#include <walle/sys/ScopeLock.h>

namespace walle {
namespace sys{

CountDown::CountDown(int c):_mutex(),_cond(),_count(c)
{
	
}
CountDown::~CountDown()
{
}
void CountDown::down()
{
	ScopeMutex lock(&_mutex);
	_count--;
	if(_count == 0) {
		_cond.broadcast();
	}
}
void CountDown::wait()
{
	ScopeMutex lock(&_mutex);
	while (_count > 0 ) {
		_cond.wait(&_mutex);
	}
}
int CountDown::getCount()
{
	ScopeMutex lock(&_mutex);
	return _count;
}

}
}
