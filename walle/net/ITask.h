#ifndef WALLE_ITASK_H_
#define WALLE_ITASK_H_

class ITask{
	public:
		ITask();
		virtual ~ITask();
		virtual run() = 0;
};
#endif
