#ifndef WALLE_APPEND_FILE_H_
#define WALLE_APPEND_FILE_H_

#include <stdint.h>
#include <string>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
using namespace std;
namespace walle {
namespace sys {    
class AppendFile{
    public:
        static const size_t kTempBuffSize = 64 * 1024;
      AppendFile(const char* filename);
      AppendFile(const string& filename);
      ~AppendFile();
      void flush();
      bool append(const char* content, const size_t len);
      size_t writeSize();
    private:
       size_t unlockWrite(const char* content, const size_t len);
       FILE  *_fp;
       size_t _writesize;
       char   _buffer[kTempBuffSize];
       
       
};
}
}
#endif
