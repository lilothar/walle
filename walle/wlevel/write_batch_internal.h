#ifndef WALLE_WLEVEL_WRITE_BATCH_INTERNAL_H_
#define WALLE_WLEVEL_WRITE_BATCH_INTERNAL_H_

#include <walle/wlevel/write_batch.h>

namespace walle {
namespace wlevel {

class MemTable;


class WriteBatchInternal {
 public:

  static int Count(const WriteBatch* batch);

  static void SetCount(WriteBatch* batch, int n);

  // Return the seqeunce number for the start of this batch.
  static SequenceNumber Sequence(const WriteBatch* batch);

  // Store the specified number as the seqeunce number for the start of
  // this batch.
  static void SetSequence(WriteBatch* batch, SequenceNumber seq);

  static Slice Contents(const WriteBatch* batch) {
    return Slice(batch->rep_);
  }

  static size_t ByteSize(const WriteBatch* batch) {
    return batch->rep_.size();
  }

  static void SetContents(WriteBatch* batch, const Slice& contents);

  static Status InsertInto(const WriteBatch* batch, MemTable* memtable);

  static void Append(WriteBatch* dst, const WriteBatch* src);
};

}

}
#endif  
