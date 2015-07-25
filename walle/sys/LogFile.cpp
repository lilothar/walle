#include <walle/sys/wallesys.h>

namespace walle{

namespace sys {

LogFile::LogFile(const string & base,
				const size_t rollsize,
				bool threadsafe, 
				const int & flushinterval,
				const int checksize )
				:_basename(base),
				_rollsize(rollsize),
				_flushstep(flushinterval),
				_checkN(checksize),
				_count(0),
				_mutex(threadsafe? new Mutex : NULL)
				
				
				
{
	roll();
}

LogFile::~LogFile(){}

void LogFile::flush()
{
	if (_mutex)
	{
	  ScopeMutex lock(&*_mutex);
	  _file->flush();
	}
	else
	{
	  _file->flush();
	}


}
void LogFile::append(const char * logline, size_t len)
{
	  if (_mutex) {
    	ScopeMutex lock(&*_mutex);
    	append_unlocked(logline, len);
  	} else {
    	append_unlocked(logline, len);
  	}
}
void LogFile::append_unlocked(const char* logline, size_t len)
{
	  _file->append(logline, len);
	
	  if (_file->writeSize() > _rollsize) {
			roll();
	  }
	  else {
		++_count;
		if (_count >= _checkN) {
		  	_count = 0;
		  	Time now = Time::now();
		  	if (now.day() != _lastroll.day() ) {
				roll();
		  	} else if (now - _lastflush > _flushstep) {
				_lastflush = now;
				_file->flush();
		  	}
		}
	  }


}

void LogFile::append(const string & logline)
{
	append(logline.c_str(), logline.length());
}

bool LogFile::roll()
{
	Time t =Time::now();
	string filename = getLogFileName(t);

	if(t.toSeconds() > _lastroll.toSeconds()) {
		_lastroll = t;
		_lastflush = t;
		_file.reset(new AppendFile(filename));
		return true;
	}
	return false;	
}

string LogFile::getLogFileName(const Time &t)
{
	string filename;
	filename.reserve(_basename.size() + 64);
	filename = _basename;
	filename += ProcessInfo::hostname();
		
	char pidbuf[32];
	snprintf(pidbuf, sizeof pidbuf, ".%d", ::getpid());
	filename += pidbuf;

	
	time_t now = t.toSeconds();
	char timebuf[32];
 	struct tm tm;
	localtime_r(&now, &tm); // FIXME: localtime_r ?
	strftime(timebuf, sizeof timebuf, ".%Y%m%d-%H%M%S", &tm);
	filename += timebuf;

	
	filename += ".log";
	
	return filename;
	
}
}
}
