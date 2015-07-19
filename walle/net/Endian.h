#ifndef ENDIAN_H_
#define ENDIAN_H_
#include <stdint.h>
#include <endian.h>
#if defined(__clang__) || __GNUC_MINOR__ >= 6
#pragma GCC diagnostic push
#endif
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wold-style-cast"
namespace walle{
namespace net{
class Endian
{
public:
static inline uint64_t hostToNetwork64(uint64_t host64)
{
  return htobe64(host64);
}

static inline uint32_t hostToNetwork32(uint32_t host32)
{
  return htobe32(host32);
}

static inline uint16_t hostToNetwork16(uint16_t host16)
{
  return htobe16(host16);
}

static inline uint64_t networkToHost64(uint64_t net64)
{
  return be64toh(net64);
}

static inline uint32_t networkToHost32(uint32_t net32)
{
  return be32toh(net32);
}

static inline uint16_t networkToHost16(uint16_t net16)
{
  return be16toh(net16);
}

};
}
}
#endif 