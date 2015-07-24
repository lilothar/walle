#ifndef WALLE_LOGFILE_H_
#define WALLE_LOGFILE_H_
#include <string>
#include <walle/sys/AppendFile.h>
#include <walle/sys/Time.h>
#include <walle/sys/Mutex.h>
#include <boost/scoped_ptr.hpp>


using std::string;

namespace walle {
namespace sys {
class LogFile{
  public:
    LogFile(const string & base,
                const size_t rollsize ,
                bool threadsafe = true,
                const int & flushinterval = 3,
                const int checksize=4096);
    ~LogFile();
    void append(const char * line, size_t len);
    void append(const string & logline);
    bool roll();
    void flush();
    
  private:
    void append_unlocked(const char* logline, size_t len);
    string getLogFileName(const Time &t);
	
    const string _basename;
    const size_t _rollsize;
    const int    _flushstep;
    const int    _checkN;
    int          _count;

    boost::scoped_ptr<Mutex> _mutex;
    Time                    _lastroll;
    Time                    _lastflush;
    boost::scoped_ptr<AppendFile> _file;
    
        
};
}
}
#endif
