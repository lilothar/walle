#include <sys/time.h>
#include <walle/sys/wallesys.h>

namespace walle{
namespace sys{

DateTime::DateTime()
{
	 struct timeval tv;
	 if(gettimeofday(&tv, 0) < 0)
	 {
		assert( 0 );
	 }
	 _time = tv.tv_sec;
	 _microsec = tv.tv_usec;
	 update_ex();
}

DateTime::DateTime(const DateTime& src)
{
	   _year = src._year;
	   _month = src._month;
	   _day = src._day;
	   _hour = src._hour;
	   _min = src._min;
	   _second = src._second ;
	   _microsec = src._microsec;
	   _wday = src._wday;
	   _time = src._time;
	   _yday =src._yday;

}

DateTime::DateTime(time_t src, long usec)
{
	struct tm ptm;
	localtime_r(&src, &ptm);

	_year = static_cast<long>(ptm.tm_year+1900);
	_month = static_cast<long>(ptm.tm_mon+1);
	_day = static_cast<long>(ptm.tm_mday);
	_hour = static_cast<long>(ptm.tm_hour);
	_min = static_cast<long>(ptm.tm_min);
	_second = static_cast<long>(ptm.tm_sec);
	_wday   = static_cast<long>(ptm.tm_wday);
	_yday   = static_cast<long>(ptm.tm_yday);
	_time = static_cast<long>(src);
	_microsec = static_cast<long>(usec);

}


DateTime::DateTime(time_t src)
{
	struct tm ptm;
	localtime_r(&src, &ptm);

	_year = static_cast<long>(ptm.tm_year+1900);
	_month = static_cast<long>(ptm.tm_mon+1);
	_day = static_cast<long>(ptm.tm_mday);
	_hour = static_cast<long>(ptm.tm_hour);
	_min = static_cast<long>(ptm.tm_min);
	_second = static_cast<long>(ptm.tm_sec);
	_wday   = static_cast<long>(ptm.tm_wday);
	_yday   = static_cast<long>(ptm.tm_yday);
	_time = static_cast<long>(src);
	_microsec = static_cast<long>(0);
}
void DateTime::update()
{
	 struct timeval tv;
	 if(gettimeofday(&tv, 0) < 0)
	 {
		assert( 0 );
	 }
	 _time = tv.tv_sec;
	 _microsec = tv.tv_usec;
	 update_ex();
}
DateTime DateTime::currentDateTime()
{
    return DateTime();
}
DateTime& DateTime::operator = (const DateTime& src)
{
	   _year = src._year;
	   _month = src._month;
	   _day = src._day;
	   _hour = src._hour;
	   _min = src._min;
	   _second = src._second ;
	   _microsec = src._microsec;
	   _wday = src._wday;
	   _time = src._time;
	   _yday =src._yday;
       
    return *this;
}

DateTime& DateTime::operator = (const time_t& rhs)
{
	struct tm ptm;
	localtime_r(&rhs, &ptm);

	_year = static_cast<long>(ptm.tm_year+1900);
	_month = static_cast<long>(ptm.tm_mon+1);
	_day = static_cast<long>(ptm.tm_mday);
	_hour = static_cast<long>(ptm.tm_hour);
	_min = static_cast<long>(ptm.tm_min);
	_second = static_cast<long>(ptm.tm_sec);
	_wday   = static_cast<long>(ptm.tm_wday);
	_yday   = static_cast<long>(ptm.tm_yday);
	_time = static_cast<long>(rhs);
	_microsec = static_cast<long>(0);
	return * this;

}
DateTime::DateTime(const string& src)
{
	*this = src;
}
long DateTime::year()
{
	return _year;
}
void DateTime::year(long y)
{
	_year = y;
	update_time();
}
void DateTime::update_time()
{
	struct tm ptm;
	 ptm.tm_year = implicit_cast<int>(_year-1900L);
	 ptm.tm_mon = implicit_cast<int>(_month - 1L);
	 ptm.tm_mday = implicit_cast<int>(_day);
	 ptm.tm_hour = implicit_cast<int>(_hour);
	 ptm.tm_min = implicit_cast<int>(_min);
	 ptm.tm_sec = implicit_cast<int>(_second);
	 _time = mktime(&ptm);
	 update_ex();
}
void DateTime::update_ex()
{
	struct tm ptm;
	localtime_r(&_time, &ptm);

	_year = ptm.tm_year+1900;
	_month = ptm.tm_mon+1;
	_day = ptm.tm_mday;
	_hour = ptm.tm_hour;
	_min = ptm.tm_min;
	_second = ptm.tm_sec;
	_wday   = ptm.tm_wday;
	_yday   = ptm.tm_yday;
}
long DateTime::month()
{
	return _month;
}
void DateTime::month(long m)
{
	_month = m;
	update_time();
}

long DateTime::day()
{
	return _day;
}
void DateTime::day(long d)
{
	_day = d;
	update_time();
}

long DateTime::hour()
{
	return _hour;
}
void DateTime::hour(long h)
{
	_hour = h;
	update_time();
}

long DateTime::min()
{
	return _min;
}
void DateTime::min(long m)
{
	_min = m;
	update_time();
}

long DateTime::second()
{
	return _second;
}
void DateTime::second(long s)
{
	_second = s;
	update_time();
}
long DateTime::time()
{
	return _time;
}
 void DateTime::time(time_t t)
 {
	 _time = t;
	 update_ex();
 }
long DateTime::microsec()
{
	return _microsec;
}
void DateTime::microsec(long m)
{
	_microsec = m;
}

DateTime DateTime::operator + (const DateTime& rhs) const { return DateTime(_time + rhs._time); }
DateTime DateTime::operator + (time_t rhs) const { return DateTime(_time + rhs); }
DateTime DateTime::operator - (const DateTime& rhs) const { return DateTime(_time - rhs._time); }
DateTime DateTime::operator - (time_t rhs) const { return DateTime(_time - rhs); }
//-----------------------------------------------------------------------------
// 描述: 将字符串转换成 DateTime
// 注意: dateTimeStr 的格式必须为 YYYY-MM-DD HH:MM:SS.*
//-----------------------------------------------------------------------------
DateTime& DateTime::operator = (const string& dateTimeStr)
{
    long y, mon, d, h, mi, sec, mic;
    const char * str;
    y= mon = d = h =  mi = sec = mic = 0;
    str = dateTimeStr.substr(0, 4).c_str();
    if (str)
    	y = static_cast<long>(walle::sys::StringUtil::atoi(str, strlen(str)));
    str = dateTimeStr.substr(5, 2).c_str();
    if (str)
    	mon = static_cast<long>(walle::sys::StringUtil::atoi(str, strlen(str)));
    str =dateTimeStr.substr(8, 2).c_str();
    if (str)
    	d = static_cast<long>(walle::sys::StringUtil::atoi(str, strlen(str)));
    str = dateTimeStr.substr(11, 2).c_str();
    if (str)
    	h = static_cast<long>(walle::sys::StringUtil::atoi(str, strlen(str)));
    str = dateTimeStr.substr(14, 2).c_str();
	if (str)
		mi = static_cast<long>(walle::sys::StringUtil::atoi(str, strlen(str)));
	str = dateTimeStr.substr(17, 2).c_str();
	if (str)
		sec = static_cast<long>(walle::sys::StringUtil::atoi(str, strlen(str)));
	if(dateTimeStr.length()>20)
	{
		str = dateTimeStr.substr(20).c_str();
		if(str) {
			mic = static_cast<long>(walle::sys::StringUtil::atoi(str, strlen(str)));
			size_t i= 6 - strlen(str);
			while(i>0)
			{
				mic *= static_cast<long>(10);
				i--;
			}
		}
	}

	encodeDateTime(y, mon, d, h, mi, sec, mic);
	return *this;

}
string DateTime::toString()
{/*
	char buff[128];
	if(_microsec) {
		sprintf(buff,"%04l-%02l-%02l %02l:%02l:%02l.%06l",_year, _month, _day,_hour, _min, _second, _microsec);
		return string(buff);
	}else {
		sprintf(buff,"%04l-%02l-%02l %02l:%02l:%02l",_year, _month, _day,_hour, _min, _second);
		return string(buff);
	}
	*/
	return string("");

}
//-----------------------------------------------------------------------------
// 描述: 日期时间编码，并存入 *this
//-----------------------------------------------------------------------------
void DateTime::encodeDateTime(long y, long mon, long d,
		long h, long mi, long sec, long mic)
{
    struct tm ptm;

    ptm.tm_year = implicit_cast<int>(y - 1900);
    ptm.tm_mon = implicit_cast<int>(mon - 1);
    ptm.tm_mday = implicit_cast<int>(d);
    ptm.tm_hour = implicit_cast<int>(h);
    ptm.tm_min = implicit_cast<int>(mi);
    ptm.tm_sec = implicit_cast<int>(sec);
    _microsec =mic;
    _time = mktime(&ptm);
    update_ex();
}

//-----------------------------------------------------------------------------
// 描述: 日期时间解码，并存入各参数
//-----------------------------------------------------------------------------
void DateTime::decodeDateTime(long *y, long *mon, long *d,
    long *h, long *mi, long *sec, long *wD, long *yD) const
{

   *y = _year;
   *mon = _month;
   *d = _day;
   *h = _hour;
   *mi = _min;
   *sec = _second;
   *wD = _wday;
   *yD = _yday;
}

//-----------------------------------------------------------------------------
// 描述: 返回日期字符串
// 参数:
//   dateSep - 日期分隔符
// 格式:
//   YYYY-MM-DD
//-----------------------------------------------------------------------------
/*
string DateTime::dateString(const string& dateSep) const
{
    string result;
    int year, month, day;

    decodeDateTime(&year, &month, &day, NULL, NULL, NULL, NULL);
    result = Util::formatstring("%04d%s%02d%s%02d",
        year, dateSep.c_str(), month, dateSep.c_str(), day);

    return result;
}
*/
//-----------------------------------------------------------------------------
// 描述: 返回日期时间字符串
// 参数:
//   dateSep     - 日期分隔符
//   dateTimeSep - 日期和时间之间的分隔符
//   timeSep     - 时间分隔符
// 格式:
//   YYYY-MM-DD HH:MM:SS
//-----------------------------------------------------------------------------
/*
string DateTime::dateTimeString(const string& dateSep,
    const string& dateTimeSep, const string& timeSep) const
{
    string result;
    int year, month, day, hour, minute, second;

    decodeDateTime(&year, &month, &day, &hour, &minute, &second, NULL);
    result = Util::formatstring("%04d%s%02d%s%02d%s%02d%s%02d%s%02d",
        year, dateSep.c_str(), month, dateSep.c_str(), day,
        dateTimeSep.c_str(),
        hour, timeSep.c_str(), minute, timeSep.c_str(), second);

    return result;
}
*/

bool DateTime::operator == (const DateTime& rhs) const { return _time == rhs._time; }
bool DateTime::operator != (const DateTime& rhs) const { return _time != rhs._time; }
bool DateTime:: operator > (const DateTime& rhs) const  { return _time > rhs._time; }
bool DateTime::operator < (const DateTime& rhs) const  { return _time < rhs._time; }
bool DateTime::operator >= (const DateTime& rhs) const { return _time >= rhs._time; }
bool DateTime::operator <= (const DateTime& rhs) const { return _time <= rhs._time; }
}
}
