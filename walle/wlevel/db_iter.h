#ifndef WALLE_WLEVEL_DB_ITER_H_
#define WALLE_WLEVEL_DB_ITER_H_

#include <stdint.h>
#include <walle/wlevel/db.h>
#include <walle/wlevel/dbformat.h>

namespace walle {
namespace wlevel {

class DBImpl;

// Return a new iterator that converts internal keys (yielded by
// "*internal_iter") that were live at the specified "sequence" number
// into appropriate user keys.
extern Iterator* NewDBIterator(
    DBImpl* db,
    const Comparator* user_key_comparator,
    Iterator* internal_iter,
    SequenceNumber sequence,
    uint32_t seed);

}  
}

#endif  
