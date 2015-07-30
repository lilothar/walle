#ifndef WALLE_MULTIMAP_H_
#define WALLE_MULTIMAP_H_

#include "functional.h"
#include "pair.h"
#include "tree.h"

namespace walle {
namespace algorithm {

template<typename Key,
         typename T,
         typename Compare = walle::algorithm::less<Key>,
         typename Alloc = allocator,
         template<typename TreeKey,
                  typename TreeValue,
                  typename TreeKeyOfvalue,
                  typename TreeCompare,
                  typename TreeAlloc = allocator > class CONT = rb_tree>
class multimap {
public:
    typedef Key key_type;
    typedef T data_type;
    typedef pair<key_type, T> value_type;    
    typedef Compare key_compare;
private:
    typedef rb_tree<key_type, value_type, select1st<value_type>, key_compare, Alloc> rep_type;
    rep_type t;
public:
  typedef typename rep_type::pointer pointer;
  typedef typename rep_type::reference reference;
  typedef typename rep_type::iterator iterator;
  typedef typename rep_type::size_type size_type;
  typedef typename rep_type::difference_type difference_type;
public:
    multimap() : t(key_compare()) {}

    key_compare key_comp() {
        return t.key_comp();
    }

    iterator begin() {
        return t.begin();
    }

    iterator end() {
        return t.end();
    }

    bool empty() {
        return t.empty();
    }

    size_type size() {
        return t.size();
    }

    iterator insert(const value_type& x) {
        return t.insert_equal(x);
    }

    void erase(iterator position) {
        t.erase(position);
    }

    size_type erase(const key_type& x) {
        return t.erase(x);
    }

    void erase(iterator first, iterator last) {
        t.erase(first, last);
    }

    void clear() {
        t.clear();
    }
};

}
}
#endif

