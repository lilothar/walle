#ifndef WALLE_STORE_OPTIONS_H_
#define WALLE_STORE_OPTIONS_H_

#include <stddef.h>

namespace leveldb {

enum CompressionType {
  kNoCompression     = 0x0,
  kSnappyCompression = 0x1
};

struct Options {
  Options();
};

// Options that control read operations
struct ReadOptions {
  ReadOptions()
  {
  }
};

// Options that control write operations
struct WriteOptions {
 

  WriteOptions()
     {
  }
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_INCLUDE_OPTIONS_H_
