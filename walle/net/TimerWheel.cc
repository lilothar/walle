#include <walle/net/TimerWheel.h>
#include <walle/net/Eventloop.h>

namespace walle {

namespace net {

namespace detail {
typedef std::function<void(std::shared_ptr<TimerTask>)> WheelFunc;
	class FuncAdapter{
	public:
		FuncAdapter(const WheelFunc &wf, 
			const std::shared_ptr<TimerTask> &ptr)
			:_task(ptr),
			_func(wf)
		{
		}
	void run()
	{
		_func(_task);	
	}
		std::shared_ptr<TimerTask> _task;
		WheelFunc   _func;


	};
	}
TimerWheel::TimerWheel(EventLoop *loop, int64_t numb)
	:_numBuckets(numb),
	 _index(0),
	 _buffer(numb,TimerWheelList()),
	 _loop(loop),
	 _timerid(),
	 _stoped(false)
{

}

TimerWheel:: ~TimerWheel()
{
	stop();
}
    
TimerWheelHander TimerWheel::addTimer(const TimerCallback&factor, int64_t interval, bool repeat)
{
	std::shared_ptr<TimerTask> wheelPtr(new TimerTask(factor, Time::now() + interval * 1000000,
											repeat?interval:0));
	TimerWheelHander ret;
	ret.ptr = wheelPtr;
	if(_loop->isInLoopThread()) {
		
		addTimerInLoop(wheelPtr);
	} else {
		detail::WheelFunc func= std::bind(&TimerWheel::addTimerInLoop,this,std::_1);
		detail::FuncAdapter fd(func,wheelPtr);
		_loop->runInLoop(std::bind(&detail::FuncAdapter::run,fd));
	}
	return ret;
}
    
void TimerWheel::start()
{
	_loop->runEvery(1000000,std::bind(&TimerWheel::onTimer, this));
}
void TimerWheel::stop()
{
	if(_stoped) {
		return ;
	}
	if(_loop->isInLoopThread()) {
		stopInLoop();
	} else {
		_loop->runInLoop(std::bind(&TimerWheel::stopInLoop, this));
	}
}

void TimerWheel::onTimer()
{
	_loop->assertInLoopThread();

	_index = (_index + 1) % _numBuckets;

	TimerWheelList mylist;
	mylist.clear();

	std::swap(mylist,_buffer[_index]);

	for(size_t i =0;i <mylist.size(); i++) {
		std::shared_ptr<TimerTask> ptr = mylist[i].lock();
		if(ptr) {
			ptr->run();
			if(ptr->repeat()) {
				ptr->restart(Time::now());
				addTimerInLoop(ptr);

			}
		}
	}

	mylist.clear();
	
}
void TimerWheel::addTimerInLoop(const std::shared_ptr<TimerTask> &task)
{
	const int64_t sec = task->step();
	size_t runIndex = (_index + sec)% _numBuckets;
	TimerWheelList & list = _buffer[runIndex];
	std::weak_ptr<TimerTask> wptr(task);
	list.push_back(wptr);	
}
void TimerWheel::stopInLoop()
{
	_stoped = true;
	_loop->cancel(_timerid);
	for(size_t k = 0; k <_buffer.size(); k++ ) {
		TimerWheelList mylist;
		mylist.clear();
	}
	
}


}

}
