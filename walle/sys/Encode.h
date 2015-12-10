#ifndef WALLE_UTILS_ENCODE_H_
#define WALLE_UTILS_ENCODE_H_
#include <walle/config/config.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>

namespace walle {
namespace sys {

inline uint16_t hton16(uint16_t num)
{
  if (SYSTEM_BIGEND) return num;
  return static_cast<uint16_t>((num & 0x00ffU) << 8) | ((num & 0xff00U) >> 8);
}

inline uint32_t hton32(uint32_t num)
{
  if (SYSTEM_BIGEND) return num;
  return ((num & 0x000000ffUL) << 24) | ((num & 0x0000ff00UL) << 8) | \
      ((num & 0x00ff0000UL) >> 8) | ((num & 0xff000000UL) >> 24);
}

inline uint64_t hton64(uint64_t num)
{
  if (SYSTEM_BIGEND) return num;
  return ((num & 0x00000000000000ffULL) << 56) | ((num & 0x000000000000ff00ULL) << 40) |
      ((num & 0x0000000000ff0000ULL) << 24) | ((num & 0x00000000ff000000ULL) << 8) |
      ((num & 0x000000ff00000000ULL) >> 8) | ((num & 0x0000ff0000000000ULL) >> 24) |
      ((num & 0x00ff000000000000ULL) >> 40) | ((num & 0xff00000000000000ULL) >> 56);
}

inline uint16_t ntoh16(uint16_t num)
{
  return hton16(num);
}

inline uint32_t ntoh32(uint32_t num)
{
  return hton32(num);
}

inline uint64_t ntoh64(uint64_t num)
{
  return hton64(num);
}

inline void writefixnum8(void* buf, uint8_t num)
{
	*(uint8_t*)buf = num;
}

inline void writefixnum16(void* buf, uint16_t num)
{
  num = hton16(num);
  memcpy(buf, (const char*)&num, sizeof(num));
}

inline void writefixnum32(void* buf, uint32_t num)
{
  num = hton32(num);
  memcpy(buf, (const char*)&num, sizeof(num));
}

inline void writefixnum64(void* buf, uint64_t num)
{
  num = hton64(num);
  memcpy(buf, (const char*)&num, sizeof(num));
}

inline uint8_t readfixnum8(const void* buf)
{
  return (*((uint8_t*)buf));
}

inline uint16_t readfixnum16(const void* buf)
{
  uint16_t num = 0;
  memcpy((char*)&num, buf, sizeof(num));
  return ntoh16(num);
}

inline uint32_t readfixnum32(const void* buf)
{
  uint32_t num = 0;
  memcpy((char*)&num, buf, sizeof(num));
  return ntoh32(num);
}

inline uint64_t readfixnum64(const void* buf)
{
  uint64_t num = 0;
  memcpy((char*)&num, buf, sizeof(num));
  return ntoh64(num);
}

inline size_t writevarnum(void* buf, uint64_t num)
{
  unsigned char* wp = (unsigned char*)buf;
  if (num < (1ULL << 7)) {
    *(wp++) = static_cast<unsigned char>(num);
  } else if (num < (1ULL << 14)) {
    *(wp++) = static_cast<unsigned char>((num >> 7) | 0x80);
    *(wp++) = static_cast<unsigned char>(num & 0x7f);
  } else if (num < (1ULL << 21)) {
    *(wp++) = static_cast<unsigned char>((num >> 14) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 7) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(num & 0x7f);
  } else if (num < (1ULL << 28)) {
    *(wp++) = static_cast<unsigned char>((num >> 21) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 14) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 7) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(num & 0x7f);
  } else if (num < (1ULL << 35)) {
    *(wp++) = static_cast<unsigned char>((num >> 28) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 21) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 14) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 7) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(num & 0x7f);
  } else if (num < (1ULL << 42)) {
    *(wp++) = static_cast<unsigned char>((num >> 35) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 28) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 21) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 14) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 7) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(num & 0x7f);
  } else if (num < (1ULL << 49)) {
    *(wp++) = static_cast<unsigned char>((num >> 42) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 35) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 28) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 21) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 14) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 7) & 0x7f) | 0x80);
    *(wp++) =static_cast<unsigned char>( num & 0x7f);
  } else if (num < (1ULL << 56)) {
    *(wp++) = static_cast<unsigned char>((num >> 49) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 42) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 35) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 28) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 21) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 14) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 7) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(num & 0x7f);
  } else if (num < (1ULL << 63)) {
    *(wp++) = static_cast<unsigned char>((num >> 56) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 49) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 42) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 35) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 28) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 21) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 14) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 7) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(num & 0x7f);
  } else {
    *(wp++) = static_cast<unsigned char>((num >> 63) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 56) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 49) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 42) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 35) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 28) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 21) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 14) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(((num >> 7) & 0x7f) | 0x80);
    *(wp++) = static_cast<unsigned char>(num & 0x7f);
  }
  return wp - (unsigned char*)buf;
}

inline size_t readvarnum(const void* buf, size_t size, uint64_t* np)
{
  const unsigned char* rp = (const unsigned char*)buf;
  const unsigned char* ep = rp + size;
  uint64_t num = 0;
  uint32_t c;
  do {
    if (rp >= ep) {
      *np = 0;
      return 0;
    }
    c = *rp;
    num = (num << 7) + (c & 0x7f);
    rp++;
  } while (c >= 0x80);
  *np = num;
  return static_cast<size_t>(rp - (const unsigned char*)buf);
}

inline size_t sizevarnum(uint64_t num)
{
  if (num < (1ULL << 7)) return 1;
  if (num < (1ULL << 14)) return 2;
  if (num < (1ULL << 21)) return 3;
  if (num < (1ULL << 28)) return 4;
  if (num < (1ULL << 35)) return 5;
  if (num < (1ULL << 42)) return 6;
  if (num < (1ULL << 49)) return 7;
  if (num < (1ULL << 56)) return 8;
  if (num < (1ULL << 63)) return 9;
  return 10;
}


}
}
#endif
