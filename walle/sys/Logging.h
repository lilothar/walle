#ifndef WALLE_SYS_LOGGING_H_
#define WALLE_SYS_LOGGING_H_
#include <walle/sys/LogStream.h>
#include <walle/sys/Time.h>
#if defined(__clang__)
    #pragma clang diagnostic ignored "-Wtautological-compare"
#else
    #pragma GCC diagnostic ignored "-Wtype-limits"
#endif

namespace walle
{

namespace sys {
class Logger
{
 public:
  enum LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS
  };

  class SourceFile
  {
   public:
    template<int N>
    inline SourceFile(const char (&arr)[N])
      : _data(arr),
        _size(N-1)
    {
      const char* slash = strrchr(_data, '/'); // builtin function
      if (slash)
      {
        _data = slash + 1;
        _size -= _data - arr;
      }
    }

    explicit SourceFile(const char* filename)
      : _data(filename)
    {
      const char* slash = strrchr(filename, '/');
      if (slash)
      {
        _data = slash + 1;
      }
      _size = static_cast<int>(strlen(_data));
    }

    const char* _data;
    size_t      _size;
  };

  Logger(SourceFile file, int line);
  Logger(SourceFile file, int line, LogLevel level);
  Logger(SourceFile file, int line, LogLevel level, const char* func);
  Logger(SourceFile file, int line, bool toAbort);
  ~Logger();

  LogStream& stream() { return _impl._stream; }

  static LogLevel logLevel();
  static void setLogLevel(LogLevel level);


  typedef void (*OutputFunc)(const char* msg, size_t len);
  typedef void (*FlushFunc)();
  static void setOutput(OutputFunc);
  static void setFlush(FlushFunc);

 private:

class Impl
{
 public:
  typedef Logger::LogLevel LogLevel;
  Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
  void formatTime();
  void finish();

  Time       _time;
  LogStream  _stream;
  LogLevel   _level;
  int        _line;
  SourceFile _basename;
};

  Impl       _impl;

};

extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::logLevel()
{
  return g_logLevel;
}



const char* strerror_tl(int savedErrno);

// Taken from glog/logging.h
//
// Check that the input is non NULL.  This very useful in constructor
// initializer lists.

#define CHECK_NOTNULL(val) \
  ::dlsys::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL().
template <typename T>
T* CheckNotNull(Logger::SourceFile file, int line, const char *names, T* ptr) {
  if (ptr == NULL) {
   Logger(file, line, Logger::FATAL).stream() << names;
  }
  return ptr;
}

}

}
#define LOG_TRACE if(walle::sys::Logger::TRACE >= walle::sys::Logger::logLevel()) \
    walle::sys::Logger(__FILE__, __LINE__, walle::sys::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if(walle::sys::Logger::DEBUG >= walle::sys::Logger::logLevel()) \
    walle::sys::Logger(__FILE__, __LINE__, walle::sys::Logger::DEBUG, __func__).stream()
#define LOG_INFO if(walle::sys::Logger::INFO >= walle::sys::Logger::logLevel()) \
    walle::sys::Logger(__FILE__, __LINE__, walle::sys::Logger::INFO,__func__).stream()
#define LOG_WARN if(walle::sys::Logger::WARN >= walle::sys::Logger::logLevel()) \
    walle::sys::Logger(__FILE__, __LINE__, walle::sys::Logger::WARN,__func__).stream()
#define LOG_ERROR walle::sys::Logger(__FILE__, __LINE__, walle::sys::Logger::ERROR, __func__).stream()
#define LOG_FATAL walle::sys::Logger(__FILE__, __LINE__, walle::sys::Logger::FATAL, __func__).stream()


#endif
