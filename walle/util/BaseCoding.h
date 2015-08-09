#ifndef WALLE_UTIL_BASE_H_
#define WALLE_UTIL_BASE_H_
#include <string>
using std::string;
namespace walle {
namespace util {
	
	inline char* baseencode(const void* buf, size_t size) {
	  assert(buf);
	  const char* tbl = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	  const unsigned char* rp = (const unsigned char*)buf;
	  char* zbuf = new char[size*4/3+5];
	  char* wp = zbuf;
	  for (size_t i = 0; i < size; i += 3) {
		switch (size - i) {
		  case 1: {
			*(wp++) = tbl[rp[0] >> 2];
			*(wp++) = tbl[(rp[0] & 3) << 4];
			*(wp++) = '=';
			*(wp++) = '=';
			break;
		  }
		  case 2: {
			*(wp++) = tbl[rp[0] >> 2];
			*(wp++) = tbl[((rp[0] & 3) << 4) + (rp[1] >> 4)];
			*(wp++) = tbl[(rp[1] & 0xf) << 2];
			*(wp++) = '=';
			break;
		  }
		  default: {
			*(wp++) = tbl[rp[0] >> 2];
			*(wp++) = tbl[((rp[0] & 3) << 4) + (rp[1] >> 4)];
			*(wp++) = tbl[((rp[1] & 0xf) << 2) + (rp[2] >> 6)];
			*(wp++) = tbl[rp[2] & 0x3f];
			break;
		  }
		}
		rp += 3;
	  }
	  *wp = '\0';
	  return zbuf;
	}

inline char* basedecode(const char* str, size_t* sp) {
  assert(str && sp);
  size_t bpos = 0;
  size_t eqcnt = 0;
  size_t len = std::strlen(str);
  unsigned char* zbuf = new unsigned char[len+4];
  unsigned char* wp = zbuf;
  size_t zsiz = 0;
  while (bpos < len && eqcnt == 0) {
    size_t bits = 0;
    size_t i;
    for (i = 0; bpos < len && i < 4; bpos++) {
      if (str[bpos] >= 'A' && str[bpos] <= 'Z') {
        bits = (bits << 6) | (str[bpos] - 'A');
        i++;
      } else if (str[bpos] >= 'a' && str[bpos] <= 'z') {
        bits = (bits << 6) | (str[bpos] - 'a' + 26);
        i++;
      } else if (str[bpos] >= '0' && str[bpos] <= '9') {
        bits = (bits << 6) | (str[bpos] - '0' + 52);
        i++;
      } else if (str[bpos] == '+') {
        bits = (bits << 6) | 62;
        i++;
      } else if (str[bpos] == '/') {
        bits = (bits << 6) | 63;
        i++;
      } else if (str[bpos] == '=') {
        bits <<= 6;
        i++;
        eqcnt++;
      }
    }
    if (i == 0 && bpos >= len) continue;
    switch (eqcnt) {
      case 0: {
        *wp++ = (bits >> 16) & 0xff;
        *wp++ = (bits >> 8) & 0xff;
        *wp++ = bits & 0xff;
        zsiz += 3;
        break;
      }
      case 1: {
        *wp++ = (bits >> 16) & 0xff;
        *wp++ = (bits >> 8) & 0xff;
        zsiz += 2;
        break;
      }
      case 2: {
        *wp++ = (bits >> 16) & 0xff;
        zsiz += 1;
        break;
      }
    }
  }
  zbuf[zsiz] = '\0';
  *sp = zsiz;
  return (char*)zbuf;
}

}

}
#endif
