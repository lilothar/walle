
#ifndef WALLE_STACK_H__
#define WALLE_STACK_H__

#include "deque.h"

namespace walle {
namespace algorithm {
	
template<class T, class Sequence = deque<T> >
class stack {
    friend bool operator==(const stack&, const stack&);
    friend bool operator<(const stack&, const stack&);
protected:
    Sequence c;
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::size_type size_type;
public:
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference top() { return c.back(); }
    void push(const value_type& x) { c. push_back(x); }
    void pop() { c.pop_back(); }
};

template<class T, class Sequence>
bool operator==(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
    return x.c == y.c;
}

template <class T, class Sequence>
bool operator<(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
  return x.c < y.c;
}

} 
}

#endif

