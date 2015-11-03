#include <walle/sys/Logging.h>
#include <walle/sys/LocalThread.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>

namespace walle
{
namespace sys {
__thread char t_errnobuf[512];
__thread char t_time[32];
__thread time_t t_lastSecond;

const char* strerror_tl(int savedErrno)
{
  return strerror_r(savedErrno, t_errnobuf, sizeof t_errnobuf);
}

Logger::LogLevel initLogLevel()
{
  if (::getenv("WALLE_LOG_TRACE")) {
   	 return Logger::TRACE;
  } else if (::getenv("WALLE_LOG_DEBUG")) {
    return Logger::DEBUG;
  } else {
    return Logger::INFO;
  }
}

Logger::LogLevel g_logLevel = initLogLevel();

const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
{
  "TRACE ",
  "DEBUG ",
  "INFO  ",
  "WARN  ",
  "ERROR ",
  "FATAL ",
};

// helper class for known string length at compile time
class T
{
 public:
  T(const char* str, unsigned len)
    :_str(str),
     _len(len)
  {
    assert(strlen(str) == _len);
  }

  const char*    _str;
  const unsigned _len;
};

inline LogStream& operator<<(LogStream& s, T v)
{
  s.append(v._str, v._len);
  return s;
}

inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v)
{
  s.append(v._data, v._size);
  return s;
}

void defaultOutput(const char* msg, size_t len)
{
  size_t n = fwrite(msg, 1, len, stdout);
  //FIXME check n
  (void)n;
}

void defaultFlush()
{
  fflush(stdout);
}

Logger::OutputFunc g_output = defaultOutput;
Logger::FlushFunc g_flush = defaultFlush;


Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file, int line)
  : _time(Time::now()),
    _stream(level >= g_logLevel?true:false),
    _level(level),
    _line(line),
    _basename(file)
{
   
  formatTime();
  LocalThread::tid();

  _stream << T(LocalThread::tidString(),LocalThread::tidStringLength())<<" ";
  _stream << T(LogLevelName[level], 6);
  _stream << " [" << _basename << ':' << _line << "] ";
  if (savedErrno != 0)
  {
    _stream << strerror_tl(savedErrno) << " (errno=" << savedErrno << ") ";
  }
}

void Logger::Impl::formatTime()
{
  int64_t microSecondsSinceEpoch = _time.toMicroSeconds();
  time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / 1000000);
  int microseconds = static_cast<int>(microSecondsSinceEpoch % 1000000);
  if (seconds != t_lastSecond)
  {
    t_lastSecond = seconds;
    struct tm tm_time;
    ::gmtime_r(&seconds, &tm_time);
    int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",
        tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
        tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    assert(len == 17); (void)len;
  }
  char us[32];
  snprintf(us, (sizeof(us) -1), ".%06dZ ", microseconds);
  _stream << T(t_time, 17) << T(us, 9);
}

void Logger::Impl::finish()
{
  _stream <<'\n';
}

Logger::Logger(SourceFile file, int line)
  : _impl(INFO, 0, file, line)
{
}

Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
  : _impl(level, 0, file, line)
{
  _impl._stream << func << ' ';
}

Logger::Logger(SourceFile file, int line, LogLevel level)
  : _impl(level, 0, file, line)
{
}

Logger::Logger(SourceFile file, int line, bool toAbort)
  : _impl(toAbort?FATAL:ERROR, errno, file, line)
{
}

Logger::~Logger()
{
  _impl.finish();/*
  const char * data = stream().buffer().data();
  size_t size = stream().buffer().size();*/
  const LogStream::Buffer& buf(stream().buffer());
  g_output(buf.data(), buf.size());
  if (_impl._level == FATAL)
  {
    g_flush();
  }
}

void Logger::setLogLevel(Logger::LogLevel level)
{
  g_logLevel = level;
}

void Logger::setOutput(OutputFunc out)
{
  g_output = out;
}

void Logger::setFlush(FlushFunc flush)
{
  g_flush = flush;
}
}
}
