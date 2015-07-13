#ifndef U_TIME_H_
#define U_TIME_H_
#include <iostream>

namespace unit{

	class Time
	{
	public:
		Time();
		Time(unsigned int sec, unsigned int usec);
		
		unsigned int seconds() const;
		unsigned int microseconds() const;
		
		static Time current();
		
		friend Time operator+(const Time& t1, const Time& t2);
		friend Time operator-(const Time& t1, const Time& t2);
		
		friend std::ostream& operator<<(std::ostream& os, const Time& t);
		
	private:
		unsigned int _sec;
		unsigned int _usec;
	};

}
#endif
