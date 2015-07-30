#ifndef WALLE_QUEUE_H__
#define WALLE_QUEUE_H__

#include "deque.h"

namespace walle {
namespace algorithm {


template<class T, class Sequence = deque<T> >
class queue {
protected:
    Sequence c;
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
public:
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference front() { return c.front(); }
    reference back() { return c.back(); }
    void push(const value_type& x) { c.push_back(x); }
    void pop() { c.pop_front(); }
};


}
}


#endif

