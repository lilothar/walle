#include <walle/sys/wallesys.h>

namespace walle {
namespace sys {
	AppendFile::AppendFile(const char* filename):
		_fp(::fopen(filename, "ae")),_writesize(0)
	{
		assert(_fp);
		::setbuffer(_fp, _buffer, sizeof _buffer);	
	}
	AppendFile::AppendFile(const string& filename):
		_fp(::fopen(filename.c_str(), "ae")),_writesize(0)
	{
		assert(_fp);
		::setbuffer(_fp, _buffer, sizeof _buffer);
	}
	AppendFile::~AppendFile()
	{
		if(_fp) {
			::fclose(_fp);
		}
	}
	void AppendFile::flush()
	{
		if(_fp) {
			::fflush(_fp);
		}
	}
	bool AppendFile::append(const char* content, const size_t len)
	{
		size_t n = unlockWrite(content, len);
		size_t remain = len -n;
		while (remain > 0) {
			n = unlockWrite(content+len-remain,remain);
			if (n == 0) {
				int err = ferror(_fp);
				if(err) {
					fprintf(stderr,  "AppendFile::append() failed %d\n", err);
					return false;
				}
			}
			remain = len -n;
		}
		_writesize += len;
		return true;

	}
	size_t AppendFile::writeSize()
	{
		return _writesize;
	}
	size_t AppendFile::unlockWrite(const char* content, const size_t len)
	{
		return ::fwrite_unlocked(content, 1, len, _fp);
	}

}
}
