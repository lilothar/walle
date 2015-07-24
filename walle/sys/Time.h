#ifndef WALLE_TIME_H_
#define WALLE_TIME_H_
#include <sys/time.h>
#include <stdint.h>
#include <string>
#include <string>
#include <vector>
using std::vector;
using std::string;

namespace walle{
namespace sys {

class Time
{
public:

    Time();

    enum Clock { Realtime, Monotonic };

    static Time now(Clock clock= Realtime);

    static Time seconds(int64_t usec);

    static Time milliSeconds(int64_t milli);

    static Time microSeconds(int64_t micro);

    static Time dateString(const string & datestr,
        const char* spliter = "- :.");

    int64_t toSeconds() const;
	int year() const;
	int day() const;
	int yday() const;
	int month() const;
    int64_t toMilliSeconds() const;

    int64_t toMicroSeconds() const;

    double toSecondsDouble() const;
    double toMilliSecondsDouble() const;
    double toMicroSecondsDouble() const;

    // if 3 or 6 bit on default on
    std::string toDateTime() const;
  

    std::string toDuration() const;
    bool valid() {return _usec !=0;}

    Time operator-() const
    {
        return Time(-_usec);
    }

    Time operator-(const Time& rhs) const
    {
        return Time(_usec - rhs._usec);
    }

    Time operator+(const Time& rhs) const
    {
        return Time(_usec + rhs._usec);
    }
    Time operator+(const int64_t& rhs) const
    {
        return Time(_usec + rhs);
    }
    Time& operator+=(const Time& rhs)
    {
        _usec += rhs._usec;
        return *this;
    }
    Time& operator+=(const int64_t & rhs)
    {
        _usec += rhs;
        return *this;
    }
    Time& operator-=(const Time& rhs)
    {
        _usec -= rhs._usec;
        return *this;
    }
    Time& operator-=(const int64_t& rhs)
    {
        _usec -= rhs;
        return *this;
    }
    bool operator<(const Time& rhs) const
    {
        return _usec < rhs._usec;
    }

    bool operator<=(const Time& rhs) const
    {
        return _usec <= rhs._usec;
    }

    bool operator>(const Time& rhs) const
    {
        return _usec > rhs._usec;
    }

    bool operator>=(const Time& rhs) const
    {
        return _usec >= rhs._usec;
    }

    bool operator==(const Time& rhs) const
    {
        return _usec == rhs._usec;
    }

    bool operator!=(const Time& rhs) const
    {
        return _usec != rhs._usec;
    }

    double operator/(const Time& rhs) const
    {
        
        return static_cast<double>(_usec) / static_cast<double>(rhs._usec);
    }

    Time& operator*=(int rhs)
    {
        _usec *= rhs;
        return *this;
    }

    Time operator*(int rhs) const
    {
        Time t;
        t._usec = _usec * rhs;
        return t;
    }

    Time& operator/=(int rhs)
    {
        _usec /= rhs;
        return *this;
    }

    Time operator/(int rhs) const
    {
        Time t;
        t._usec = _usec / rhs;
        return t;
    }

    Time& operator*=(int64_t rhs)
    {
        _usec *= rhs;
        return *this;
    }

    Time operator*(int64_t rhs) const
    {
        Time t;
        t._usec = _usec * rhs;
        return t;
    }

    Time& operator/=(int64_t rhs)
    {
        _usec /= rhs;
        return *this;
    }
    operator timeval() const;
    Time operator/(int64_t rhs) const
    {
        Time t;
        t._usec = _usec / rhs;
        return t;
    }

    Time& operator*=(double rhs)
    {
        _usec = static_cast<int64_t>(static_cast<double>(_usec) * rhs);
        return *this;
    }

    Time operator*(double rhs) const
    {
        Time t;
        t._usec = static_cast<int64_t>(static_cast<double>(_usec) * rhs);
        return t;
    }

    Time& operator/=(double rhs)
    {
        _usec = static_cast<int64_t>(static_cast<double>(_usec) / rhs);
        return *this;
    }

    
    Time operator/(double rhs) const
    {
        Time t;
        t._usec = static_cast<int64_t>(static_cast<double>(_usec) / rhs);
        return t;
    }
   Time(int64_t usec);
    static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
	struct tm _ptm;
    int64_t   _usec;
};
}
}
#endif
