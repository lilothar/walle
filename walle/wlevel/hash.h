#ifndef WALLE_WLEVEL_HASH_H_
#define WALLE_WLEVEL_HASH_H_

#include <stddef.h>
#include <stdint.h>

namespace walle {
namespace wlevel {

extern uint32_t Hash(const char* data, size_t n, uint32_t seed);

}
}

#endif  // STORAGE_LEVELDB_UTIL_HASH_H_
