#ifndef WALLE_SKIPLIST_H_
#define WALLE_SKIPLIST_H_
#include <stddef.h>
#include "alloc.h"
#include "constructor.h"
#include "iterator.h"
#include "Random.h"

namespace walle {

namespace algorithm {



template <typename T>  
struct default_comparetor {
    int operator() (const T& l, const T& r)
    {
        if(l < r) {
            return -1;
        } else if(r < l) {
            return 1;
        }

        return 0;
    }

};

template <typename KEY>
class skiplist_node {
    public:
       const KEY                   data;
       const int                   height;
       skiplist_node<KEY>         *prev;
       skiplist_node<KEY>         *next;

};


template <typename KEY, typename REF, typename PTR>

struct skiplist_iterator {
    typedef skiplist_iterator<KEY, KEY&, KEY*> iterator;
    typedef skiplist_iterator<KEY, const KEY&, const KEY*> const_iterator;
    typedef skiplist_iterator<KEY, REF, PTR> self;

    typedef bidirectional_iterator_tag iterator_category;
    typedef KEY                        value_type;
    typedef ptrdiff_t                  difference_type;
    typedef REF                        reference;
    typedef PTR                        pointer;
    typedef skiplist_node<KEY>*        link_type;
    typedef size_t                     size_type;          

    link_type  node;
    
    skiplist_iterator(){}
    ~skiplist_iterator(){}
    bool operator==(const self& x) const { return node == x.node; }
    bool operator!=(const self& x) const { return node != x.node; }
    reference operator*() const { return (*node).data; }
    pointer operator->() const { return &(operator*()); }
    self& operator++() {
        node = (link_type)((*node).next);
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }
    self& operator--() {
        node = (link_type)((*node).prev);
        return *this;
    }
    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }

}; 


template<typename K, typename V, typename E,
    typename Compare = default_comparetor<K>, typename Alloc = allocator>

class skiplist{
public:
    typedef skiplist_iterator<V, V&, V*> iterator;
    typedef skiplist_iterator<V, const V&, const V*> const_iterator;
    typedef V      value_type;
    typedef ptrdiff_t difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef const value_type* const_pointer;
    typedef const value_type& const_reference;
    typedef skiplist_node<V>  node_type;
    typedef skiplist_node<V>* link_type;
    typedef size_t            size_type;
    typedef default_alloc<skiplist_node<V>, Alloc> list_node_allocator; 
public:
    skiplist()
        :_maxheigth(kDefaultheigth),
         _head(get_node(0,_maxheigth)),
         _tail(get_node(0,_maxheigth)),
         _size(0)
         
    {
        for(int i = 0; i < _maxheigth; i++ ) {
            _head->next[i] = _tail;     
        }
        _tail->prev = _head;
        _head->prev = NULL;
    }

       
    ~skiplist()
    {
        link_type *l = _head;
        link_type *tmp ;
        while(l != _tail) {
            tmp = l->next[0];
            destory_node(l);
            l = tmp;
        
        }
        destory_node(_tail);
        

    }

    iterator begin() { return (link_type*)(_head->next); }
    iterator end() {return _tail; }

    void clear()
    {
        link_type *l = _head->next;
        link_type *tmp ;
        while(l != _tail) {
            tmp = l->next[0];
            destory_node(l);
            l = tmp;
        
        }

        _size = 0;

    }

    void erase(iterator itr)
    {
       remove(*itr);   
    }

    void insert(const value_type& v)
    {
        int h = _rad.oneIn(_maxheigth);
        link_type ln = create_node(v, h);
        link_type updateArray[_maxheigth];
        int k = _maxheigth - 1;
        link_type p = _head;
        link_type q;
        do {
            while (q = p->next[k] &&
                _cmper(_ex(q->data),_ex(v)) < 0 ) {

                p = q;
            }
            updateArray[k] = p; 
        } while(--k > 0);
        // insert to p-v-q;
        k = ln->height - 1;
        do {
            p = updateArray[k];
            ln->next[k] = p->next[k];
            p->next[k] = ln;
            ln->prev = updateArray[0];
        } while(--k >= 0);
        

    }
    void remove(const value_type& v)
    {

        link_type updateArray[_maxheigth];
        int k = _maxheigth - 1;
        link_type p = _head;
        link_type q;
        do {
            while (q = p->next[k] &&
                _cmper(_ex(q->data),_ex(v)) < 0 ) {

                p = q;
            }
            updateArray[k] = p; 
        } while(--k > 0);

        //found
        if(_cmper(_ex(q->data),_ex(v)) == 0) {
            k = q->height - 1;
            do {
                p = updateArray[k];
                p->next[k] = q->next[k];
        
            } while (--k >= 0);
            q->next->prev = q->prev;
        }
               

    }

    iterator find(const value_type& v)
    {
        link_type p = _head;
        link_type q; 
        int k = _maxheigth -1 ;
        do {
            while (q = p->next[k] &&
                _cmper(_ex(q->data),_ex(v)) < 0 ) {

                p = q;
            }
        
        }while (--k >= 0);
        // p < v <= q
        if(_cmper(_ex(q->data),_ex(v)) == 0) {
            return q;
        }
        //not found
        return _tail;
    }
    
    iterator find_lower(const value_type& v)
    {
        link_type p = _head;
        link_type q; 
        int k = _maxheigth;
        do {
            while (q = p->next[k] &&
                _cmper(_ex(q->data),_ex(v)) < 0 ) {

                p = q;
            }
        
        }while (--k >= 0);
        // p < v <= q
        
        //not found
        return p;   
    }

    iterator find_upper(const value_type& v)
    {
        link_type p = _head;
        link_type q; 
        int k = _maxheigth;
        do {
            while (q = p->next[k] &&
                _cmper(_ex(q->data),_ex(v)) < 0 ) {

                p = q;
            }
        
        }while (--k >= 0);
        // p < v <= q
        
        //not found
        return p->next;   

    }
    size_type size() { return _size; }

    bool empty() { return _size == 0; }
private:
    skiplist(const skiplist&);
    void operator=(const skiplist&);
protected:
    link_type get_node(int h)
    {
        size_t memsize = sizeof(node_type) +sizeof(link_type)*(h -1);
         link_type link = list_node_allocator::allocate(memsize);
         return link;
    }

    link_type create_node(const value_type & v, int h)
    {
        link_type mem =  get_node(h);
        construct(&mem->data,v);
        mem->height = h;
        return mem;
    }

    void free_node(link_type n)
    {
        list_node_allocator::deallocate(n);
    }

    void destory_node(link_type n) 
    {
        destory(&n->data);
        free_node(n);
    }
    
protected:
   const int        _maxheigth;
   Compare    _cmper;
   E          _ex;
   link_type  _head;
   link_type  _tail;
   size_type  _size;
   Random     _rad;
   static const int kDefaultheigth = 12;
   
   
};

}
}
#endif
