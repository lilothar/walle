#ifndef WALLE_READ_SMALL_FILE_H_
#define WALLE_READ_SMALL_FILE_H_
#include <stdint.h>
#include <string>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
using namespace std;

namespace walle{
namespace sys {

class ReadSmallFile{
    public:
    static const size_t kmaxsmallreadsize = 128*1024;
    ReadSmallFile();
    ReadSmallFile(const string &filename);
    ReadSmallFile(const char* filename);
    ~ReadSmallFile();
    bool read(string *content, size_t maxlen);
    
    private:
       FILE   *_fp;
       char    _buff[kmaxsmallreadsize];
};  
}
}
#endif
