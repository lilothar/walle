#include <walle/store/util.h>
#include <walle/store/myconf.h>

namespace walle {
namespace store {


/** The package version. */
const char* const VERSION = "0.1";


/** The library version. */
const int32_t LIBVER = 0;


/** The library revision. */
const int32_t LIBREV = 1;


/** The database format version. */
const int32_t FMTVER = 1;


/** The flag for big endian environments. */
const bool BIGEND =  false;

const int32_t CLOCKTICK = sysconf(_SC_CLK_TCK);
const int32_t PAGESIZ = sysconf(_SC_PAGESIZE);


/** The extra feature list. */
const char* const FEATURES = "zlib";


// get the levenshtein distance of two arrays
template<class CHARTYPE, class CNTTYPE>
static size_t levdist(const CHARTYPE* abuf, size_t asiz, const CHARTYPE* bbuf, size_t bsiz) {
  size_t dsiz = bsiz + 1;
  size_t tsiz = (asiz + 1) * dsiz;
  CNTTYPE tblstack[2048/sizeof(CNTTYPE)];
  CNTTYPE* tbl = tsiz > sizeof(tblstack) / sizeof(*tblstack) ? new CNTTYPE[tsiz] : tblstack;
  tbl[0] = 0;
  for (size_t i = 1; i <= asiz; i++) {
    tbl[i*dsiz] = i;
  }
  for (size_t i = 1; i <= bsiz; i++) {
    tbl[i] = i;
  }
  abuf--;
  bbuf--;
  for (size_t i = 1; i <= asiz; i++) {
    for (size_t j = 1; j <= bsiz; j++) {
      uint32_t ac = tbl[(i-1)*dsiz+j] + 1;
      uint32_t bc = tbl[i*dsiz+j-1] + 1;
      uint32_t cc = tbl[(i-1)*dsiz+j-1] + (abuf[i] != bbuf[j]);
      ac = ac < bc ? ac : bc;
      tbl[i*dsiz+j] = ac < cc ? ac : cc;
    }
  }
  size_t ed = tbl[asiz*dsiz+bsiz];
  if (tbl != tblstack) delete[] tbl;
  return ed;
}


/**
 * Calculate the levenshtein distance of two regions in bytes.
 */
size_t memdist(const void* abuf, size_t asiz, const void* bbuf, size_t bsiz) {
  assert(abuf && asiz <= MEMMAXSIZ && bbuf && bsiz <= MEMMAXSIZ);
  return asiz > UINT8MAX || bsiz > UINT8MAX ?
    levdist<const char, uint32_t>((const char*)abuf, asiz, (const char*)bbuf, bsiz) :
    levdist<const char, uint8_t>((const char*)abuf, asiz, (const char*)bbuf, bsiz);
}


/**
 * Calculate the levenshtein distance of two UTF-8 strings.
 */
size_t strutfdist(const char* astr, const char* bstr) {
  assert(astr && bstr);
  size_t anum = strutflen(astr);
  uint32_t astack[128];
  uint32_t* aary = anum > sizeof(astack) / sizeof(*astack) ? new uint32_t[anum] : astack;
  strutftoucs(astr, aary, &anum);
  size_t bnum = strutflen(bstr);
  uint32_t bstack[128];
  uint32_t* bary = bnum > sizeof(bstack) / sizeof(*bstack) ? new uint32_t[bnum] : bstack;
  strutftoucs(bstr, bary, &bnum);
  size_t dist = strucsdist(aary, anum, bary, bnum);
  if (bary != bstack) delete[] bary;
  if (aary != astack) delete[] aary;
  return dist;
}


/**
 * Calculate the levenshtein distance of two UCS-4 arrays.
 */
size_t strucsdist(const uint32_t* aary, size_t anum, const uint32_t* bary, size_t bnum) {
  assert(aary && anum <= MEMMAXSIZ && bary && bnum <= MEMMAXSIZ);
  return anum > UINT8MAX || bnum > UINT8MAX ?
    levdist<const uint32_t, uint32_t>(aary, anum, bary, bnum) :
    levdist<const uint32_t, uint8_t>(aary, anum, bary, bnum);
}


/**
 * Allocate a nullified region on memory.
 */
void* mapalloc(size_t size) {
  assert(size > 0 && size <= MEMMAXSIZ);
  void* ptr = ::mmap(0, sizeof(size) + size,
                     PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (ptr == MAP_FAILED) throw std::bad_alloc();
  *(size_t*)ptr = size;
  return (char*)ptr + sizeof(size);
}


/**
 * Free a region on memory.
 */
void mapfree(void* ptr) {
  assert(ptr);
  size_t size = *((size_t*)ptr - 1);
  ::munmap((char*)ptr - sizeof(size), sizeof(size) + size);
}


/**
 * Get the time of day in seconds.
 * @return the time of day in seconds.  The accuracy is in microseconds.
 */
double time() {
  assert(true);
  struct ::timeval tv;
  if (::gettimeofday(&tv, NULL) != 0) return 0.0;
  return tv.tv_sec + tv.tv_usec / 1000000.0;
}


/**
 * Get the process ID.
 */
int64_t getpid() {
  assert(true);
  return ::getpid();
}


/**
 * Get the value of an environment variable.
 */
const char* getenv(const char* name) {
  assert(name);
  return ::getenv(name);
}


/**
 * Get system information of the environment.
 */
void getsysinfo(std::map<std::string, std::string>* strmap) {
  assert(strmap);
  struct ::rusage rbuf;
  std::memset(&rbuf, 0, sizeof(rbuf));
  if (::getrusage(RUSAGE_SELF, &rbuf) == 0) {
    (*strmap)["ru_utime"] = strprintf("%0.6f",
                                      rbuf.ru_utime.tv_sec + rbuf.ru_utime.tv_usec / 1000000.0);
    (*strmap)["ru_stime"] = strprintf("%0.6f",
                                      rbuf.ru_stime.tv_sec + rbuf.ru_stime.tv_usec / 1000000.0);
    if (rbuf.ru_maxrss > 0) {
      int64_t size = rbuf.ru_maxrss * 1024LL;
      (*strmap)["mem_peak"] = strprintf("%lld", (long long)size);
      (*strmap)["mem_size"] = strprintf("%lld", (long long)size);
      (*strmap)["mem_rss"] = strprintf("%lld", (long long)size);
    }
  }
  std::ifstream ifs;
  ifs.open("/proc/self/status", std::ios_base::in | std::ios_base::binary);
  if (ifs) {
    std::string line;
    while (getline(ifs, line)) {
      size_t idx = line.find(':');
      if (idx != std::string::npos) {
        const std::string& name = line.substr(0, idx);
        idx++;
        while (idx < line.size() && line[idx] >= '\0' && line[idx] <= ' ') {
          idx++;
        }
        const std::string& value = line.substr(idx);
        if (name == "VmPeak") {
          int64_t size = atoix(value.c_str());
          if (size > 0) (*strmap)["mem_peak"] = strprintf("%lld", (long long)size);
        } else if (name == "VmSize") {
          int64_t size = atoix(value.c_str());
          if (size > 0) (*strmap)["mem_size"] = strprintf("%lld", (long long)size);
        } else if (name == "VmRSS") {
          int64_t size = atoix(value.c_str());
          if (size > 0) (*strmap)["mem_rss"] = strprintf("%lld", (long long)size);
        }
      }
    }
    ifs.close();
  }
  ifs.open("/proc/meminfo", std::ios_base::in | std::ios_base::binary);
  if (ifs) {
    std::string line;
    while (getline(ifs, line)) {
      size_t idx = line.find(':');
      if (idx != std::string::npos) {
        const std::string& name = line.substr(0, idx);
        idx++;
        while (idx < line.size() && line[idx] >= '\0' && line[idx] <= ' ') {
          idx++;
        }
        const std::string& value = line.substr(idx);
        if (name == "MemTotal") {
          int64_t size = atoix(value.c_str());
          if (size > 0) (*strmap)["mem_total"] = strprintf("%lld", (long long)size);
        } else if (name == "MemFree") {
          int64_t size = atoix(value.c_str());
          if (size > 0) (*strmap)["mem_free"] = strprintf("%lld", (long long)size);
        } else if (name == "Cached") {
          int64_t size = atoix(value.c_str());
          if (size > 0) (*strmap)["mem_cached"] = strprintf("%lld", (long long)size);
        }
      }
    }
    ifs.close();
  }
}


/**
 * Set the standard streams into the binary mode.
 */
void setstdiobin() 
{
}


}                                        // common namespace
}

