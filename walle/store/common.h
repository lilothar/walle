#ifndef WALLE_STORE_COMMON_H        
#define WALLE_STORE_COMMON_H

extern "C" {
#include <stdint.h>
}

#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <cstring>
#include <ctime>
#include <cwchar>

#include <stdexcept>
#include <exception>
#include <limits>
#include <new>
#include <typeinfo>

#include <utility>
#include <functional>
#include <memory>
#include <iterator>
#include <algorithm>
#include <locale>

#include <string>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <map>
#include <set>

#include <ios>
#include <iostream>
#include <streambuf>
#include <fstream>
#include <sstream>

namespace std {
using ::modfl;
using ::snprintf;
}


#include <tr1/unordered_map>
#include <tr1/unordered_set>

namespace std {
using tr1::hash;
using tr1::unordered_map;
using tr1::unordered_set;
}

#undef VERSION
#undef LIBVER
#undef LIBREV
#undef OSNAME
#undef BIGEND
#undef CLOCKTICK
#undef PAGESIZ
#undef FEATURES
#undef NUMBUFSIZ
#undef MEMMAXSIZ

#undef IOBUFSIZ
#undef SUCCESS
#undef NOIMPL
#undef INVALID
#undef NOREPOS
#undef NOPERM
#undef BROKEN
#undef DUPREC
#undef NOREC
#undef LOGIC
#undef SYSTEM
#undef MISC

#undef DEBUG
#undef INFO
#undef WARN
#undef ERROR
#undef OPEN
#undef CLOSE
#undef CLEAR
#undef ITERATE
#undef SYNCHRONIZE
#undef OCCUPY
#undef BEGINTRAN
#undef COMMITTRAN
#undef ABORTTRAN

#undef INT8MAX
#undef INT16MAX
#undef INT32MAX
#undef INT64MAX
#undef INT8MIN
#undef INT16MIN
#undef INT32MIN
#undef INT64MIN
#undef UINT8MAX
#undef UINT16MAX
#undef UINT32MAX
#undef UINT64MAX
#undef SIZEMAX
#undef FLTMAX
#undef DBLMAX

#define CODELINE  __FILE__, __LINE__, __FUNCTION__

#endif            

