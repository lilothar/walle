
#include "Utime.h"
#include <sys/time.h>


using namespace std;

namespace unit
{
	
	const unsigned int UsecPerSec = 1000000;
		
	Time::Time()
	:	_sec(0),
		_usec(0)
	{}
	

	Time::Time(unsigned int sec, unsigned int usec)
	:	_sec(sec),
		_usec(usec)
	{}
	

	unsigned int
	Time::seconds() const
	{
		return _sec;
	}
	

	unsigned int
	Time::microseconds() const
	{
		return _usec;
	}
	

	Time
	Time::current()
	{
		struct timeval tv;
		gettimeofday(&tv, 0);
		return Time((unsigned int)tv.tv_sec, (unsigned int)tv.tv_usec);
	}
	
	Time
	operator-(const Time& t1, const Time& t2)
	{
		if (t2._sec > t1._sec || (t2._sec == t1._sec && t2._usec > t1._usec))
			return Time();
		
		unsigned int sec = t1._sec - t2._sec;
		unsigned int usec;
		
		if (t2._usec > t1._usec)
		{
			--sec;
			usec = UsecPerSec - (t2._usec - t1._usec);
		}
		else
			usec = t1._usec - t2._usec;
		
		return Time(sec, usec);
	}
	

	Time
	operator+(const Time& t1, const Time& t2)
	{
		unsigned int sec  = t1._sec + t2._sec;
		unsigned int usec = t1._usec + t2._usec;
		
		if (usec > UsecPerSec)
		{
			++sec;
			usec -= UsecPerSec;
		}
		return Time(sec, usec);
	}
	

	ostream&
	operator<<(ostream& os, const Time& t)
	{
		int old_fill((int)os.fill());
		int old_width((int)os.width());

		os << t.seconds() << '.';
		os.fill('0');
		os.width(6);
		os << t.microseconds();

		os.fill((char)old_fill);
		os.width(old_width);

		return os;
	}
	
}

