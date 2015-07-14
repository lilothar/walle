#ifndef WALLE_STRINGUTIL_H_
#define WALLE_STRINGUTIL_H_
#include <stdint.h>
#include <stddef.h>
#include <string>
#include <algorithm>

using std::string;
namespace walle{
namespace sys {
class StringUtil{
public:
   static int64_t atoi(const char *buff, size_t len);
   static int64_t atoix(const char *buff, size_t len);
   static int64_t atoih(const char *buff, size_t len);
   static int64_t atof(const char *buff, size_t len);
   static char *  toUpper(char* buff, size_t len);
   static char *  toLower(char* buff, size_t len);
   static char *  trim(char *str, const char *what = " ", int mode = 3);
   static int     hashcode(const char *str, size_t len);
   static int64_t hashmurmur(const void *buff, size_t len);
   template <typename T>
   static string  formateInteger(const T &value)
   {
        const char digits[] = "9876543210123456789";
	    const char* zero = digits + 9;
        char buf[128];
        T i = value;
	    char* p = buf;
	
	  do {
		int lsd = static_cast<int>(i % 10);
		i /= 10;
		*p++ = zero[lsd];
	  } while (i != 0);
	
	  if (value < 0) {
		*p++ = '-';
	  }
	  *p = '\0';
	  std::reverse(buf, p);
	
	  return string(buf, p - buf);
        
   }
   
   static string formateAddrHex(uintptr_t value)
   {
        const char digitsHex[] = "0123456789ABCDEF";
        char buf[128];
        uintptr_t i = value;
	    char* p = buf;
	
	    do {
		    int lsd = i % 16;
		    i /= 16;
		    *p++ = digitsHex[lsd];
	    } while (i != 0);
	
	    *p = '\0';
	    std::reverse(buf, p);
	
	  return string(buf, p - buf);
   }
   
   static string  formateDouble(const double &value);
};
}
}
#endif
