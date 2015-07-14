#include <walle/sys/wallesys.h>


namespace walle {
namespace sys {
int64_t StringUtil::atoi(const char *buff, size_t len)
{
	const char *str = buff;
	const char *end = buff+len;
	while (str !=end && *str > '\0' && *str <= ' ') {
    	str++;
  	}
	if(str == end) {
		return 0L;
	}
  	int32_t sign = 1;
  	int64_t num = 0;
	
  	if (*str == '-') {
    	str++;
    	sign = -1;
  	} else if (*str == '+') {
    	str++;
  	}
  	while (str != end && *str != '\0') {
    	if (*str < '0' || *str > '9') 
			break;
   		num = num * 10 + *str - '0';
    	str++;
  }
  return num * sign;
}
int64_t StringUtil::atoix(const char *buff, size_t len)
{
	const char *str = buff;
	const char *end = buff + len;
	while (str != end && *str > '\0' && *str <= ' ') {
	   str++;
	 }
	 int32_t sign = 1;
	 if(str == end) {
		return 0;
	 }
	 if (*str == '-') {
	   str++;
	   sign = -1;
	 } else if (*str == '+') {
	   str++;
	 }
	 long double num = 0;
	 while (str != end && *str != '\0') {
	   if (*str < '0' || *str > '9') break;
	   num = num * 10 + *str - '0';
	   str++;
	 }
	 if (str != end && *str == '.') {
	   str++;
	   long double base = 10;
	   while (str != end && *str != '\0') {
		 if (*str < '0' || *str > '9') break;
		 num += (*str - '0') / base;
		 str++;
		 base *= 10;
	   }
	 }
	 num *= sign;
	 while (str != end && *str > '\0' && *str <= ' ') {
	   str++;
	 }
	 if (str != end) {
	 
		 if (*str == 'k' || *str == 'K') {
		   num *= 1LL << 10;
		 } else if (*str == 'm' || *str == 'M') {
		   num *= 1LL << 20;
		 } else if (*str == 'g' || *str == 'G') {
		   num *= 1LL << 30;
		 } else if (*str == 't' || *str == 'T') {
		   num *= 1LL << 40;
		 } else if (*str == 'p' || *str == 'P') {
		   num *= 1LL << 50;
		 } else if (*str == 'e' || *str == 'E') {
		   num *= 1LL << 60;
		 }
	 }
	 return static_cast<int64_t>(num);
}
int64_t StringUtil::atoih(const char *buff, size_t len)
{
	const char *str = buff;
	const char *end = buff + len;
	while (str != end && *str > '\0' && *str <= ' ') {
    	str++;
  	}
  	if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
    	str += 2;
  	}
  	int64_t num = 0;
 	 while (str != end) {
    	if (*str >= '0' && *str <= '9') {
      		num = num * 0x10 + *str - '0';
    	} else if (*str >= 'a' && *str <= 'f') {
      		num = num * 0x10 + *str - 'a' + 10;
    	} else if (*str >= 'A' && *str <= 'F') {
     		 num = num * 0x10 + *str - 'A' + 10;
    	} else {
      		break;
   		}
    	str++;
  	}
  return num;
}
int64_t StringUtil::atof(const char *buff, size_t len)
{
	const char *ptr = buff;
	size_t size = len;
  	while (size > 0 && *ptr >= '\0' && *ptr <= ' ') {
    	ptr++;
    	size--;
 	}
  	int32_t sign = 1;
  	if (size > 0) {
    	if (*ptr == '-') {
     		ptr++;
      		size--;
      		sign = -1;
    	} else if (*ptr == '+') {
      		ptr++;
      		size--;
    	}
	}

	if (size > 2) {
		if ((ptr[0] == 'i' || ptr[0] == 'I') && (ptr[1] == 'n' || ptr[1] == 'N') &&
        	(ptr[2] == 'f' || ptr[2] == 'F')) return 0;
    	if ((ptr[0] == 'n' || ptr[0] == 'N') && (ptr[1] == 'a' || ptr[1] == 'A') &&
        	(ptr[2] == 'n' || ptr[2] == 'N')) return 0;
	}
  	long double num = 0;
  	int32_t col = 0;
  	while (size > 0) {
    	if (*ptr < '0' || *ptr > '9') break;
    	num = num * 10 + *ptr - '0';
    	ptr++;
    	size--;
    	if (num > 0) col++;
  	}

	if (size > 0 && *ptr == '.') {
    	ptr++;
    	size--;
    long double fract = 0.0;
    long double base = 10;
    while (col < 16 && size > 0) {
    	if (*ptr < '0' || *ptr > '9') break;
      	fract += (*ptr - '0') / base;
      	ptr++;
      	size--;
      	col++;
      	base *= 10;
    }
    num += fract;
  }
  if (size > 0 && (*ptr == 'e' || *ptr == 'E')) {
    ptr++;
    size--;
	long double base = 10.0;
    num *= std::pow(base, static_cast<long double>(atoi(ptr, size)));
  }
  return static_cast<double>(num * sign);
}
char *  StringUtil::toUpper(char* buff, size_t len)
{
	if (!buff || len == 0) {
		return buff;
	}
	char *p = buff;
	size_t size = len;
	while (size) {
		if(*p & 0x80) {
			p++;
		} else if(*p > 'a' && *p <= 'z'){
			*p -=32;
			
		}
		p++;
		size--;
	}

	return buff;

}
char *  StringUtil::toLower(char* buff, size_t len)
{
	if (!buff || len == 0) {
		return buff;
	}
	char *p = buff;
	size_t size = len;
	while (size) {
		//chinese two bytes
		if(*p & 0x80) {
			p++;
		} else if(*p >= 'A' && *p <= 'Z') {
			*p +=32;
		}
		p++;
		size--;
	}

	return buff;


}

char *  StringUtil::trim(char *str, const char *what, int mode)
{
	unsigned char mask[256];
	const unsigned char *q = (const unsigned char *)what;
	memset(mask, 0, sizeof(mask));
	while (*q) {
		mask[*q] = '\1';
		q++;
	}
	unsigned char * ret = (unsigned char *) str;
	//pre
	unsigned char *p;
	if(mode == 1) {
		p = ret;
		while (*p) {
			if(!*p & mask[*p]) {
				break;
			}
			p++;
			ret++;
		}

	}
	// tail
	if(mode == 2) {
		p = ret + strlen((const char*)ret);
		while (p >= ret) {
			if(!*p & mask[*p]) {
				break;
			}
			*p = '\0';
			 p--;
		}
		p++;
		*p = '\0';

	}
	return (char*)ret;

}
int StringUtil:: hashcode(const char *str, size_t len)
{
	int h = 0;
	const char *p = str;
	size_t size = len;
	
	while(size > 0 && *p) {
		h = 31*h + (*p);
		p ++;
		size--;
	}
	return h;

}
string  StringUtil::formateDouble(const double &value)
{
	char buff[32];
	snprintf(buff, sizeof(buff)-1, "%.12g",value);
	return string(buff);
}
int64_t StringUtil::hashmurmur(const void *buff, size_t len)
{
	const uint64_t mul = 0xc6a4a7935bd1e995ULL;
  	const int32_t rtt = 47;
	size_t size = len;
  	uint64_t hash = 19780211ULL ^ (size * mul);
  	const unsigned char* rp = (const unsigned char*)buff;
  	while (size >= sizeof(uint64_t)) {
    	uint64_t num = ((uint64_t)rp[0] << 0) | ((uint64_t)rp[1] << 8) |
        	((uint64_t)rp[2] << 16) | ((uint64_t)rp[3] << 24) |
        	((uint64_t)rp[4] << 32) | ((uint64_t)rp[5] << 40) |
        	((uint64_t)rp[6] << 48) | ((uint64_t)rp[7] << 56);
    	num *= mul;
    	num ^= num >> rtt;
    	num *= mul;
    	hash *= mul;
    	hash ^= num;
    	rp += sizeof(uint64_t);
    	size -= sizeof(uint64_t);
 	 }
  	switch (size) {
    	case 7: hash ^= (uint64_t)rp[6] << 48;
   	 	case 6: hash ^= (uint64_t)rp[5] << 40;
    	case 5: hash ^= (uint64_t)rp[4] << 32;
    	case 4: hash ^= (uint64_t)rp[3] << 24;
    	case 3: hash ^= (uint64_t)rp[2] << 16;
    	case 2: hash ^= (uint64_t)rp[1] << 8;
    	case 1: hash ^= (uint64_t)rp[0];
      	hash *= mul;
  	};
  	hash ^= hash >> rtt;
  	hash *= mul;
  	hash ^= hash >> rtt;
  	return hash;
}
}
}
