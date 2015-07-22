#include <walle/sys/Time.h>
#include <assert.h>
#include <sstream>
#include <string.h>
#include <iomanip>
#include <walle/sys/StringUtil.h>

namespace walle{
namespace sys {

Time::Time():
    _usec(0)
{
}
Time Time:: dateString(const string & datestr, const char* spliter)
{
	struct tm ptm;
	
	if(datestr.empty()) {
		return Time(0); 
	}
	memset(&ptm, 0, sizeof(ptm));
	vector<int64_t> dateint;
	StringUtil::stringSplitInt(datestr, spliter, dateint);
	if(dateint.size() < 6) {
		return Time(0);
	}
	ptm.tm_year = dateint[0]- 1900;
    ptm.tm_mon = dateint[1] - 1;
    ptm.tm_mday = dateint[2];
    ptm.tm_hour = dateint[3];
    ptm.tm_min = dateint[4];
    ptm.tm_sec = dateint[5];
	int64_t t = mktime(&ptm);
	int64_t t2 = time(NULL);
	t2 *= 1;
	int64_t usec = t * kMicroSecondsPerSecond;
	if(dateint.size() == 7) {
		usec += dateint[6];
	}
	return Time(usec);		
}

Time Time::now(Clock clock)
{
    if(clock == Realtime)
    {
        struct timeval tv;
        if(gettimeofday(&tv, 0) < 0)
        {
            assert( 0 );
        }
        return Time(tv.tv_sec * uint64_t(1000000) + tv.tv_usec);
    }
    else // Monotonic
    {
        struct timespec ts;
        if(clock_gettime(CLOCK_MONOTONIC, &ts) < 0)
        {
            assert(0);
        }
        return Time(ts.tv_sec * int64_t(1000000) + ts.tv_nsec / int64_t(1000));
    }
}


Time Time::seconds(int64_t t)
{
    return Time(t * uint64_t(1000000));
}

Time Time::milliSeconds(int64_t t)
{
    return Time(t * int64_t(1000));
}

Time Time::microSeconds(int64_t t)
{
    return Time(t);
}

Time::operator timeval() const
{
    timeval tv;
    tv.tv_sec = static_cast<long>(_usec / 1000000);
    tv.tv_usec = static_cast<long>(_usec % 1000000);
    return tv;
}

int64_t Time::toSeconds() const
{
    return _usec / 1000000;
}

int64_t Time::toMilliSeconds() const
{
    return _usec / 1000;
}

int64_t Time::toMicroSeconds() const
{
    return _usec;
}

double Time::toSecondsDouble() const
{
    return static_cast<double>(_usec)/ 1000000.0;
}

double Time::toMilliSecondsDouble() const
{
    return static_cast<double>(_usec) / 1000.0;
}

double Time::toMicroSecondsDouble() const
{
    return static_cast<double>(_usec);
}

std::string Time::toDateTime() const
{
    time_t time = static_cast<long>(_usec / 1000000);

    struct tm* t;
    struct tm tr;
    localtime_r(&time, &tr);
    t = &tr;

    char buf[32];
    strftime(buf, sizeof(buf), "%F %H:%M:%S", t);
    //strftime(buf, sizeof(buf), "%x %H:%M:%S", t);

    std::ostringstream os;
    os << buf << ".";
    os.fill('0');
	
	os.width(6);
	
    os << static_cast<long>(_usec % 1000000);
    return os.str();
}

std::string Time::toDuration() const
{
	int64_t usecs = _usec % 1000000;
	int64_t secs = _usec / 1000000 % 60;
	int64_t mins = _usec / 1000000 / 60 % 60;
	int64_t hours = _usec / 1000000 / 60 / 60 % 24;
	int64_t days = _usec / 1000000 / 60 / 60 / 24;

    using namespace std;

    ostringstream os;
    if(days != 0)
    {
        os << days << "d ";
    }
    os << setfill('0') << setw(2) << hours << ":" << setw(2) << mins << ":" << setw(2) << secs;
    if(usecs != 0)
    {
        os << "." << setw(3) << (usecs / 1000);
    }

    return os.str();
}

Time::Time(int64_t usec):
    _usec(usec)
{
}

}
}

