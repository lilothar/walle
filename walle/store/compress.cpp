#include <walle/store/compress.h>
#include <walle/store/myconf.h>

extern "C" {
#include <zlib.h>
}

namespace walle {            
namespace store {

/**
 * Compress a serial data.
 */
char* ZLIB::compress(const void* buf, size_t size, size_t* sp, Mode mode) {
  assert(buf && size <= MEMMAXSIZ && sp);
  z_stream zs;
  zs.zalloc = Z_NULL;
  zs.zfree = Z_NULL;
  zs.opaque = Z_NULL;
  switch (mode) {
    default: {
      if (deflateInit2(&zs, 6, Z_DEFLATED, -15, 9, Z_DEFAULT_STRATEGY) != Z_OK) return NULL;
      break;
    }
    case DEFLATE: {
      if (deflateInit2(&zs, 6, Z_DEFLATED, 15, 9, Z_DEFAULT_STRATEGY) != Z_OK) return NULL;
      break;
    }
    case GZIP: {
      if (deflateInit2(&zs, 6, Z_DEFLATED, 15 + 16, 9, Z_DEFAULT_STRATEGY) != Z_OK) return NULL;
      break;
    }
  }
  const char* rp = (const char*)buf;
  size_t zsiz = size + size / 8 + 32;
  char* zbuf = new char[zsiz+1];
  char* wp = zbuf;
  zs.next_in = (Bytef*)rp;
  zs.avail_in = size;
  zs.next_out = (Bytef*)wp;
  zs.avail_out = zsiz;
  if (deflate(&zs, Z_FINISH) != Z_STREAM_END) {
    delete[] zbuf;
    deflateEnd(&zs);
    return NULL;
  }
  deflateEnd(&zs);
  zsiz -= zs.avail_out;
  zbuf[zsiz] = '\0';
  if (mode == RAW) zsiz++;
  *sp = zsiz;
  return zbuf;
}


/**
 * Decompress a serial data.
 */
char* ZLIB::decompress(const void* buf, size_t size, size_t* sp, Mode mode) {
  assert(buf && size <= MEMMAXSIZ && sp);
  size_t zsiz = size * 8 + 32;
  while (true) {
    z_stream zs;
    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;
    switch (mode) {
      default: {
        if (inflateInit2(&zs, -15) != Z_OK) return NULL;
        break;
      }
      case DEFLATE: {
        if (inflateInit2(&zs, 15) != Z_OK) return NULL;
        break;
      }
      case GZIP: {
        if (inflateInit2(&zs, 15 + 16) != Z_OK) return NULL;
        break;
      }
    }
    char* zbuf = new char[zsiz+1];
    zs.next_in = (Bytef*)buf;
    zs.avail_in = size;
    zs.next_out = (Bytef*)zbuf;
    zs.avail_out = zsiz;
    int32_t rv = inflate(&zs, Z_FINISH);
    inflateEnd(&zs);
    if (rv == Z_STREAM_END) {
      zsiz -= zs.avail_out;
      zbuf[zsiz] = '\0';
      *sp = zsiz;
      return zbuf;
    } else if (rv == Z_BUF_ERROR) {
      delete[] zbuf;
      zsiz *= 2;
    } else {
      delete[] zbuf;
      break;
    }
  }
  return NULL;
}


/**
 * Calculate the CRC32 checksum of a serial data.
 */
uint32_t ZLIB::calculate_crc(const void* buf, size_t size, uint32_t seed) {
  assert(buf && size <= MEMMAXSIZ);
  return crc32(seed, (unsigned char*)buf, size);
}


ZLIBCompressor<ZLIB::RAW> zlibrawfunc;
ZLIBCompressor<ZLIB::RAW>* const ZLIBRAWCOMP = &zlibrawfunc;


}                                        // common namespace
}
// END OF FILE
