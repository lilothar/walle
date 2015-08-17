#ifndef WALLE_STORE_COMPRESS_H                    // duplication check
#define WALLE_STORE_COMPRESS_H

#include <walle/store/common.h>
#include <walle/store/util.h>
#include <walle/store/thread.h>

namespace walle {
namespace store {


class Compressor {
 public:
  /**
   * Destructor.
   */
  virtual ~Compressor() {}
  /**
   * Compress a serial data.
   * @param buf the input buffer.
   * @param size the size of the input buffer.
   * @param sp the pointer to the variable into which the size of the region of the return
   * value is assigned.
   * @return the pointer to the result data, or NULL on failure.
   * @note Because the region of the return value is allocated with the the new[] operator, it
   * should be released with the delete[] operator when it is no longer in use.
   */
  virtual char* compress(const void* buf, size_t size, size_t* sp) = 0;
  /**
   * Decompress a serial data.
   * @param buf the input buffer.
   * @param size the size of the input buffer.
   * @param sp the pointer to the variable into which the size of the region of the return
   * value is assigned.
   * @return the pointer to the result data, or NULL on failure.
   * @note Because an additional zero code is appended at the end of the region of the return
   * value, the return value can be treated as a C-style string.  Because the region of the
   * return value is allocated with the the new[] operator, it should be released with the
   * delete[] operator when it is no longer in use.
   */
  virtual char* decompress(const void* buf, size_t size, size_t* sp) = 0;
};


/**
 * ZLIB compressor.
 */
class ZLIB {
 public:
  /**
   * Compression modes.
   */
  enum Mode {
    RAW,                                 ///< without any checksum
    DEFLATE,                             ///< with Adler32 checksum
    GZIP                                 ///< with CRC32 checksum and various meta data
  };
  /**
   * Compress a serial data.
   * @param buf the input buffer.
   * @param size the size of the input buffer.
   * @param sp the pointer to the variable into which the size of the region of the return
   * value is assigned.
   * @param mode the compression mode.
   * @return the pointer to the result data, or NULL on failure.
   * @note Because the region of the return value is allocated with the the new[] operator, it
   * should be released with the delete[] operator when it is no longer in use.
   */
  static char* compress(const void* buf, size_t size, size_t* sp, Mode mode = RAW);
  /**
   * Decompress a serial data.
   * @param buf the input buffer.
   * @param size the size of the input buffer.
   * @param sp the pointer to the variable into which the size of the region of the return
   * value is assigned.
   * @param mode the compression mode.
   * @return the pointer to the result data, or NULL on failure.
   * @note Because an additional zero code is appended at the end of the region of the return
   * value, the return value can be treated as a C-style string.  Because the region of the
   * return value is allocated with the the new[] operator, it should be released with the
   * delete[] operator when it is no longer in use.
   */
  static char* decompress(const void* buf, size_t size, size_t* sp, Mode mode = RAW);
  /**
   * Calculate the CRC32 checksum of a serial data.
   * @param buf the input buffer.
   * @param size the size of the input buffer.
   * @param seed the cyclic seed value.
   * @return the CRC32 checksum.
   */
  static uint32_t calculate_crc(const void* buf, size_t size, uint32_t seed = 0);
};


/**
 * Compressor with ZLIB.
 */
template <ZLIB::Mode MODE>
class ZLIBCompressor : public Compressor {
 private:
  /**
   * Compress a serial data.
   */
  char* compress(const void* buf, size_t size, size_t* sp) {
    assert(buf && size <= MEMMAXSIZ && sp);
    return ZLIB::compress(buf, size, sp, MODE);
  }
  /**
   * Decompress a serial data.
   */
  char* decompress(const void* buf, size_t size, size_t* sp) {
    assert(buf && size <= MEMMAXSIZ && sp);
    return ZLIB::decompress(buf, size, sp, MODE);
  }
};


/**
 * Prepared pointer of the compressor with ZLIB raw mode.
 */
extern ZLIBCompressor<ZLIB::RAW>* const ZLIBRAWCOMP;


}                                        // common namespace
}
#endif                                   // duplication check

// END OF FILE
