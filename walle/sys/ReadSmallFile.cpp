#include <walle/sys/wallesys.h>

#include <string.h>
#include <sys/file.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


namespace walle {
namespace sys {
    ReadSmallFile::ReadSmallFile():_fp(NULL)
    {
        
    }
    ReadSmallFile::ReadSmallFile(const string &filename):_fp(NULL)
    {
        _fp = fopen(filename.c_str(), "rb");   
    }
    ReadSmallFile::ReadSmallFile(const char* filename)
    {
        _fp = fopen(filename, "rb");  
    }
    ReadSmallFile::~ReadSmallFile()
    {
        if (_fp) {
            ::fclose(_fp);
            _fp =NULL;
        }
    }
    bool ReadSmallFile::read(string *content, size_t maxSize)
    {
      if(!_fp || !content || maxSize <=0) {
		return false;
	}
	int fd =fileno(_fp);
	size_t fileSize;
	content->clear();
 	struct stat statbuf;
    if (::fstat(fd, &statbuf) == 0) {
        if (S_ISREG(statbuf.st_mode)) {
          fileSize = statbuf.st_size;
          content->reserve(static_cast<int>(std::min(maxSize, fileSize)));
        }
        else if (S_ISDIR(statbuf.st_mode)) {
          return false;
        }
    } else {
    	return false;
    }
	while (content->size() < maxSize) {
		size_t toRead = std::min(maxSize - content->size(), sizeof(_buff));
      	ssize_t n = ::read(fd, _buff, toRead);
      	if (n > 0) {
        	content->append(_buff, n);
      	} else {
        	if (n < 0) {
          		return false;
        	}
        	break;
     	}
	}
	return true;
    }


}
}
