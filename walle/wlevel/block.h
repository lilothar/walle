#ifndef WALLE_WLEVEL_BLOCK_H_
#define WALLE_WLEVEL_BLOCK_H_

#include <stddef.h>
#include <stdint.h>
#include <walle/wlevel/iterator.h>

namespace walle {
namespace wlevel {

struct BlockContents;
class Comparator;

class Block {
 public:
  // Initialize the block with the specified contents.
  explicit Block(const BlockContents& contents);

  ~Block();

  size_t size() const { return size_; }
  Iterator* NewIterator(const Comparator* comparator);

 private:
  uint32_t NumRestarts() const;

  const char* data_;
  size_t size_;
  uint32_t restart_offset_;     // Offset in data_ of restart array
  bool owned_;                  // Block owns data_[]

  // No copying allowed
  Block(const Block&);
  void operator=(const Block&);

  class Iter;
};

}  // namespace leveldb
}
#endif  // STORAGE_LEVELDB_TABLE_BLOCK_H_
