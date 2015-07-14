
#ifndef DATETIME_H_
#define DATETIME_H_
#include <time.h>
#include <string>
using std::string;

namespace walle{
namespace sys {

class DateTime
{
public:
    DateTime();
    DateTime(const DateTime& src);
    DateTime(const string& src);
    explicit DateTime(time_t src);
    explicit DateTime(time_t src, long usec);

    static DateTime currentDateTime();

    DateTime& operator = (const DateTime& rhs);

    DateTime& operator = (const time_t& rhs);
    //-----------------------------------------------------------------------------
  	//format  YYYY-MM-DD HH:MM:SS.*
    //-----------------------------------------------------------------------------
    DateTime& operator = (const string& dateTimeStr);

    void update();
    long year();
    void year(long y);

    long month();
    void month(long m);

    long day();
    void day(long d);

    long hour();
    void hour(long h);

    long min();
    void min(long m);

    long second();
    void second(long s);

    long microsec();
    void microsec(long m);
    long time();
    void time(time_t t);

    DateTime operator + (const DateTime& rhs) const;
    DateTime operator + (time_t rhs) const;
    DateTime operator - (const DateTime& rhs) const;
    DateTime operator - (time_t rhs) const;

    bool operator == (const DateTime& rhs) const;
    bool operator != (const DateTime& rhs) const;
    bool operator > (const DateTime& rhs) const;
    bool operator < (const DateTime& rhs) const;
    bool operator >= (const DateTime& rhs) const;
    bool operator <= (const DateTime& rhs) const;

    //-----------------------------------------------------------------------------
    // 描述: 将字符串转换成 DateTime
    // 注意: dateTimeStr 的格式必须为 YYYY-MM-DD HH:MM:SS.*
    //-----------------------------------------------------------------------------

    string toString();
    void encodeDateTime(long year, long month, long day,
        long hour = 0, long minute = 0, long second = 0,long micsec =0);
    void decodeDateTime(long *year, long *month, long *day,
        long *hour, long *minute, long *second,
        long *weekDay = NULL, long *yearDay = NULL) const;

private:

    void update_time();
    void update_ex();
    long   _year;
    long   _month;
    long   _day;
    long   _hour;
    long   _min;
    long   _second;
    long   _microsec;
    long   _wday;
    long   _yday;
    time_t _time;
};
}
}
#endif /* DATETIME_H_ */
