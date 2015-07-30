
#ifndef WALLE_MAP_H__
#define WALLE_MAP_H__

#include "functional.h"
#include "pair.h"
#include "tree.h"

namespace walle {
namespace algorithm {

template<typename Key,
         typename T,
         typename Compare = htl::less<Key>,
         typename Alloc = allocator,
         template<typename TreeKey,
                  typename TreeValue,
                  typename TreeKeyOfvalue,
                  typename TreeCompare,
                  typename TreeAlloc = allocator > class CONT = rb_tree>
class map {
public:
    typedef Key key_type;
    typedef T data_type;
    typedef pair<key_type, T> value_type;    // RB-TREE????
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
    map() : t(key_compare()) {}

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

    pair<iterator,bool> insert(const value_type& x) {
        return t.insert_unique(x);
    }

    T& operator[](const key_type& k) {
        // insert?????pair,first?iterator,second?iterator?second,???T()
        return (*(insert(value_type(k, T())).first)).second;
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

