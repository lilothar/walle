#ifndef WALLE_WLEVEL_MERGER_H_
#define WALLE_WLEVEL_MERGER_H_

namespace walle {
namespace wlevel {

class Comparator;
class Iterator;

extern Iterator* NewMergingIterator(
    const Comparator* comparator, Iterator** children, int n);

} 
}

#endif
