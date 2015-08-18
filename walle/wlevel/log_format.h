#ifndef WALLE_WLEVEL_LOG_FORMAT_H_
#define WALLE_WLEVEL_LOG_FORMAT_H_

namespace walle {
namespace wlevel {
namespace log {

enum RecordType {
  // Zero is reserved for preallocated files
  kZeroType = 0,

  kFullType = 1,

  // For fragments
  kFirstType = 2,
  kMiddleType = 3,
  kLastType = 4
};
static const int kMaxRecordType = kLastType;

static const int kBlockSize = 32768;

// Header is checksum (4 bytes), type (1 byte), length (2 bytes).
static const int kHeaderSize = 4 + 1 + 2;

}  // namespace log
}  // namespace leveldb
}
#endif  // STORAGE_LEVELDB_DB_LOG_FORMAT_H_
