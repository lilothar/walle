#ifndef WALLE_TREE_H_
#define WALLE_TREE_H_

#include "alloc.h"
#include "pair.h"
#include "algo.h"
#include <iostream>

namespace walle {

namespace algorithm {
	typedef bool rb_tree_color_type;
	const rb_tree_color_type rb_tree_red = false;
	const rb_tree_color_type rb_tree_black = true;

	struct rb_tree_node_base {
    rb_tree_color_type color;      
    rb_tree_node_base* parent;    
    rb_tree_node_base* left;       
    rb_tree_node_base* right;      

    static rb_tree_node_base* minimum(rb_tree_node_base* x) {
        while (x->left != NULL)
            x = x->left;
        return x;
    }

    static rb_tree_node_base* maximum(rb_tree_node_base* x) {
        while (x->right != NULL)
            x = x->right;
        return x;
    }
};


template <typename Value>
struct rb_tree_node : public rb_tree_node_base {
    typedef rb_tree_node<Value>* link_type;
    Value value_filed; 

struct rb_tree_base_iterator {
    typedef bidirectional_iterator_tag iterator_category;
    typedef rb_tree_node_base* base_ptr;
    typedef ptrdiff_t difference_type;

    base_ptr node;
    void increment() {
        if (node->right != NULL) {
            node = node->right;
            while (node->left != NULL)
                node = node->left;
        }
        else {
            base_ptr y = node->parent;
            while (node == y->right) {
                node = y;
                y = y->parent;
            }
            if (node->right != y)   
                node = y;          
        }
    }
    void decrement() {
        if (node->color == rb_tree_red        
            && node->parent->parent == node)  
            node = node->right;
        else if (node->left != NULL) {
            node = node->left;
            while (node->right != NULL)
                node = node->right;
        }
        else {
            base_ptr y = node->parent;
       
            while (node == y->left) {
                node = y;
                y = y->parent;
            }
            node = y;
        }
    }
};

template<typename Value, typename Ref, typename Ptr>
struct rb_tree_iterator : public rb_tree_base_iterator {
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef rb_tree_iterator<Value, Value&, Value*> iterator;
    typedef rb_tree_iterator<Value, Ref, Ptr> self;
    typedef rb_tree_node<Value>* link_type;

    rb_tree_iterator() {}
    rb_tree_iterator(link_type x){
        node = x;
    }
    rb_tree_iterator(const iterator& it) {
        node = it.node;
    }

    reference operator*() const {
        return link_type(node)->value_filed;
    }

    pointer operator->() const {
        return &(operator*());
    }

    self& operator++() {
        increment();
        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        increment();
        return tmp;
    }

    self& operator--() {
        decrement();
        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        decrement();
        return tmp;
    }

};

inline bool operator==(const rb_tree_base_iterator& x,
                       const rb_tree_base_iterator& y) {
  return x.node == y.node;
}

inline bool operator!=(const rb_tree_base_iterator& x,
                       const rb_tree_base_iterator& y) {
  return x.node != y.node;
}

// RB-TREE
template<typename Key,
         typename Value,
         typename KeyOfValue,
         typename Compare,
         typename Alloc = allocator>
class rb_tree {
protected:
    typedef rb_tree_node_base* base_ptr;
    typedef rb_tree_node<Value> rb_tree_node;
    typedef default_alloc<rb_tree_node, Alloc> rb_tree_node_allocator;
    typedef rb_tree_color_type color_type;
public:
    typedef Value value_type;
    typedef Key key_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef rb_tree_node* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef rb_tree_iterator<value_type, reference, pointer> iterator;
protected:

    link_type get_node() {
        return rb_tree_node_allocator::allocate();
    }

    void put_node(link_type p) {
        return rb_tree_node_allocator::deallocate(p);
    }


    link_type create_node(const value_type& x) {
        link_type tmp = get_node();
        construct(&tmp->value_filed, x);
        return tmp;
    }


    void destory_node(link_type p) {
        destory(&p->value_filed);
        put_node(p);
    }


    link_type clone_node(link_type x) {
        link_type tmp = create_node(x->value_field);
        tmp->color = x->color;
        tmp->left = 0;
        tmp->right = 0;
        tmp->parent = 0;
        return tmp;
    }
protected:

    size_type node_count;  
    link_type header;       
    Compare key_compare;  
protected:
 
    link_type& root() const {
        return (link_type&) header->parent;
    }


    link_type& leftmost() const {
        return (link_type&) header->left;
    }


    link_type& rightmost() const {
        return (link_type&) header->right;
    }


    static link_type& left(link_type x) {
        return (link_type&) (x->left);
    }

    static link_type& right(link_type x) {
        return (link_type&) (x->right);
    }

    static link_type& parent(link_type x) {
        return (link_type&) (x->parent);
    }

    static reference value(link_type x) {
        return x->value_field;
    }

    static const key_type& key(link_type x){
        return KeyOfValue()(x->value_filed);
    }

    static color_type& color(link_type x) {
        return (color_type&) (x->color);
    }

    // ??6???(??base_ptr)??????x????
    static link_type& left(base_ptr x) {
        return (link_type&) (x->left);
    }

    static link_type& right(base_ptr x) {
        return (link_type&) (x->right);
    }

    static link_type parent(base_ptr x) {
        return (link_type&) (x->parent);
    }

    static reference value(base_ptr x) {
        return ((link_type)x)->value_filed;
    }

    static const key_type& key(base_ptr x) {
        return KeyOfValue()(value(x));
    }

    static color_type& color(base_ptr x) {
        return (color_type&) (x->color);
    }

    // ??????node
    static link_type minimum(link_type x) {
        return (link_type) rb_tree_node_base::minimum(x);
    }

    static link_type maximum(link_type x) {
        return (link_type&) rb_tree_node_base::maximum(x);
    }
private:
    // ???
    void init() {
        header = get_node();
        color(header) = rb_tree_red;    
        root() = 0;
        leftmost() = header;           
        rightmost() = header;           
    }
public:
    rb_tree(const Compare& comp = Compare())
     : node_count(0), key_compare(comp) {
        init();
    }
    ~rb_tree() {
        clear();
        put_node(header);
    }
public:
    Compare key_comp() const {
        return key_compare;
    }


    iterator begin() {
        return leftmost();
    }


    iterator end() {
        return header;
    }

    bool empty() const {
        return node_count == 0;
    }

    size_type size() const {
        return node_count;
    }


    link_type get_root() {
        return root();
    }

    void mid_visit(link_type p);


    iterator insert_equal(const value_type& v);


    pair<iterator, bool> insert_unique(const value_type& v);


    void erase(iterator pos);


    size_type erase(const Key& x);


    void erase(iterator first, iterator last);


    iterator find(const key_type& k);


    void clear();


    pair<iterator,iterator> equal_range(const key_type& x);


    iterator lower_bound(const key_type& x);


    iterator upper_bound(const key_type& x);


    size_type count(const Key& k);
private:

    iterator insert(link_type x, link_type y, const value_type& v);


    void insert_fixup(base_ptr z);


    void erase_fixup(base_ptr x_parent, base_ptr x);


    void left_rotate(base_ptr x);


    void right_rotate(base_ptr y);

  
    void erase_aux(link_type x);


    base_ptr tree_successor(base_ptr x);


    void print_node(link_type p) {
        std::cout << p <<" Value : " <<
            p->value_filed << " " << "color : ";
        std::cout << (p->color ?  "black" : "red") << std::endl;
    }
};

template<typename Key, typename Value, typename KeyOfValue, typename Compare,
         typename Alloc>
void rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::mid_visit(link_type p) {
    if (p == NULL) {
        std::cout << "has no node\n";
        return;
    }
	if (p->left) {
		if (((link_type)p->left)->value_filed > ((link_type)p)->value_filed)
			std::cout << "wrong left\n";
		assert(((link_type)p->left)->value_filed <= ((link_type)p)->value_filed);
		mid_visit((link_type)p->left);
	}
	print_node(p);
	if (p->right) {
		if (((link_type)p->right)->value_filed < ((link_type)p)->value_filed)
			std::cout << "wrong right\n";
		assert(((link_type)p->right)->value_filed >= ((link_type)p)->value_filed);
		mid_visit((link_type)p->right);
	}
}
template<typename Key, typename Value, typename KeyOfValue, typename Compare,
         typename Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::insert_equal(const value_type& v) {
    link_type y = header;
    link_type x = root();
    while (x != NULL) {
        y = x;
        x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
    }
    return insert(x, y, v);
}

template<typename Key, typename Value, typename KeyOfValue, typename Compare,
         typename Alloc>
pair<typename rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::iterator, bool>
rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::insert_unique(const value_type& v) {
    link_type y = header;
    link_type x = root();
    bool comp = true;   // ????????????
    while (x != NULL) {
        y = x;
        comp = key_compare(KeyOfValue()(v), key(x));
        x = comp ? left(x) : right(x);
    }
    iterator it = iterator(y); 
    if (comp) {
        
        if (it == begin())
            return pair<iterator, bool>(insert(x, y, v), true);
        else
            --it;
    }

    if (key_compare(key(it.node), KeyOfValue()(v))) 
        return pair<iterator, bool>(insert(x, y, v), true);

    return pair<iterator, bool>(it, false);
}
template<typename Key, typename Value, typename KeyOfValue, typename Compare,
         typename Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::insert(link_type x, link_type y, const value_type& v) {
    link_type z = create_node(v);
    if (y == header) {

        root() = z;
        left(y) = z;
        right(y) = z;
    }
    else if (key_compare(KeyOfValue()(v), key(y))){

        left(y) = z;

        if (y == leftmost())
            leftmost() = z;
    }
    else {

        right(y) = z;
        if (y == rightmost())
            rightmost() = z;
    }
    parent(z) = y;
    left(z) = 0;
    right(z) = 0;

    color(z) = rb_tree_red;

    insert_fixup(z);
    ++node_count;

    return iterator(z);
}


template<typename Key, typename Value, typename KeyOfValue, typename Compare,
         typename Alloc>
void rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::insert_fixup(base_ptr z) {
    while (z != root() && z->parent->color == rb_tree_red) {
		if (z->parent == z->parent->parent->left) {
			base_ptr y = z->parent->parent->right;
			if (y && y->color == rb_tree_red) {	
				z->parent->color = rb_tree_black;
				y->color = rb_tree_black;
				z->parent->parent->color = rb_tree_red;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->right) { 
					z = z->parent;
					left_rotate(z);
				}

				z->parent->color = rb_tree_black;
				z->parent->parent->color = rb_tree_red;
				right_rotate(z->parent->parent);
			}
		}
		else if (z->parent->parent != NULL) { 
			base_ptr y = z->parent->parent->left;
			if (y && y->color == rb_tree_red) {
				z->parent->color = rb_tree_black;
				y->color = rb_tree_black;
				z->parent->parent->color = rb_tree_red;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->left) {
					z = z->parent;
					right_rotate(z);
				}
				z->parent->color = rb_tree_black;
				z->parent->parent->color = rb_tree_red;
				left_rotate(z->parent->parent);
			}
		}
	} // end while
	root()->color = rb_tree_black;
}

template<typename Key, typename Value, typename KeyOfValue, typename Compare,
         typename Alloc>
void rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::left_rotate(base_ptr x) {
    base_ptr y = x->right;
	x->right = y->left;
	if (y->left != NULL)
		y->left->parent = x;
	y->parent = x->parent;
	if (x == root()) {
		root() = (link_type)y;
	}
	else {
		if (x == x->parent->left)
			x->parent->left = y;
		else // x == x->p->right
			x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}


template<typename Key, typename Value, typename KeyOfValue, typename Compare,
         typename Alloc>
void rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::right_rotate(base_ptr y) {
	base_ptr x = y->left;
	y->left = x->right;
	if (x->right != NULL)
		x->right->parent = y;
	x->parent = y->parent;
	if (y == root()) {
		root() = (link_type)x;
	}
	else {
		if (y == y->parent->right)
			y->parent->right = x;
		else
			y->parent->left = x;
	}
	x->right = y;
	y->parent = x;
}


template<typename Key, typename Value, typename KeyOfValue, typename Compare,
         typename Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::base_ptr
rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::tree_successor(base_ptr x) {
    if (x->right != NULL)
		return rb_tree_node_base::minimum(x->right);
	base_ptr y = x->parent;
	while (y != root() && x == y->right) {
		x = y;
		y = y->parent;
	}
	return y;
}


template<typename Key, typename Value, typename KeyOfValue, typename Compare,
         typename Alloc>
void rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::erase(iterator pos) {
    base_ptr z = pos.node;
	base_ptr x_parent = NULL;
	base_ptr y = NULL;
	if (z->left == NULL || z->right == NULL)
		y = z;
	else
		y = tree_successor(z);
	base_ptr x = NULL;
	if (y->left != NULL)
		x = y->left;
	else
		x = y->right;
	if (x != NULL)
		x->parent = y->parent;
	if (y == root()) {
		root() = (link_type) x;
		x_parent = header;
	}
	else {
		if (y == y->parent->left)
			y->parent->left = x;
		else
			y->parent->right = x;
		x_parent = y->parent;
	}
	if (y != z) {
		((link_type)z)->value_filed = ((link_type)y)->value_filed;
	}
	else {  // y == z
        if ((((base_ptr)(header))->left) == y) 
            if (y->right == NULL)  
                ((base_ptr)(header))->left = y->parent;
            else   
                ((base_ptr)(header))->left = rb_tree_node_base::minimum(x);
        if ((((base_ptr)(header))->right) == y)
            if (y->left == NULL)
                ((base_ptr)(header))->right = y->parent;
            else
                ((base_ptr)(header))->right = rb_tree_node_base::maximum(x);
    }
	if (y->color == rb_tree_black)	
        erase_fixup(x_parent, x);
    destory_node((link_type)y);
    --node_count;
}


template<typename Key, typename Value, typename KeyOfValue, typename Compare,
         typename Alloc>
void rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::erase_fixup(base_ptr x_parent, base_ptr x) {
    while (x != root() && (x == NULL || x->color == rb_tree_black)) {
		if (x == x_parent->left) {
			base_ptr w = x_parent->right;
			if (w->color == rb_tree_red) {	
				w->color = rb_tree_black;
				x_parent->color = rb_tree_red;
				left_rotate(x_parent);
				// ??????
				w = x_parent->right;
			}
			if ((w->left == NULL || w->left->color == rb_tree_black)
				&& (w->right == NULL || w->right->color == rb_tree_black)) {
				w->color = rb_tree_red;
				x = x_parent;
				x_parent = x_parent->parent;
			}
			else {
				if (w->right == NULL || w->right->color == rb_tree_black) {
					w->left->color = rb_tree_black;
					w->color = rb_tree_red;
					right_rotate(w);
					w = x_parent->right;
				}
				// ??(4)
				w->color = x_parent->color;
				x_parent->color = rb_tree_black;
				w->right->color = rb_tree_black;
				left_rotate(x_parent);
				x = root();
			}
		}
		else { 
			base_ptr w = x_parent->left;
			if (w->color == rb_tree_red) {	
				w->color = rb_tree_black;
				x_parent->color = rb_tree_red;
				right_rotate(x_parent);
		
				w = x_parent->left;
			}
			if ((w->right == NULL || w->right->color == rb_tree_black)
				&& (w->left == NULL || w->left->color == rb_tree_black)) {
				w->color = rb_tree_red;
				x = x_parent;	
				x_parent = x_parent->parent;
			}
			else {
				if (w->left == NULL || w->left->color == rb_tree_black) { 
					w->right->color = rb_tree_black;
					w->color = rb_tree_red;
					left_rotate(w);
					w = x_parent->left;
				}
				// ??(4)
				w->color = x_parent->color;
				x_parent->color = rb_tree_black;
				w->left->color = rb_tree_black;
				right_rotate(x_parent);
				x = root();
			}
		}
	}
	if (x != NULL)
		x->color = rb_tree_black;
}

template<typename Key, typename Value, typename KeyOfValue, typename Compare,
         typename Alloc>
void rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::erase_aux(link_type x) {
    while (x != NULL) {
        erase_aux(right(x));
        link_type y = left(x);
        destory_node(x);
        x = y;
    }
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first,
                                                            iterator last) {
    if (first == begin() && last == end())
        clear();
    else
        while (first != last)
            erase(first++);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const Key& x) {
    pair<iterator,iterator> p = equal_range(x);
    size_type n = 0;
    distance(p.first, p.second, n);
    erase(p.first, p.second);
    return n;
}

template<typename Key, typename Value, typename KeyOfValue, typename Compare,
         typename Alloc>
void rb_tree<Key, Value, KeyOfValue,
Compare, Alloc>::clear() {
    if (node_count != 0) { 
        erase_aux(root());
        leftmost() = header;
        root() = 0;
        rightmost() = header;
        node_count = 0;
    }
}

template<typename Key, typename Value, typename KeyOfValue, typename Compare,
         typename Alloc>
typename rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::find(const key_type& k) {
    link_type y = header;  
    link_type x = root();
    while (x != NULL) {
        if (!key_compare(key(x), k)) 
            y = x, x = left(x);
        else   
            x = right(x);
    }
    iterator it = iterator(y);
    return (it == end() || key_compare(k, key(it.node))) ? end() : it;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const Key& k) {
    link_type y = header; 
    link_type x = root();
    while (x != 0)
        if (!key_compare(key(x), k))
            y = x, x = left(x);
        else
            x = right(x);
    return iterator(y);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const Key& k) {
    link_type y = header;
    link_type x = root();
    while (x != 0)
        if (key_compare(k, key(x)))
            y = x, x = left(x);
        else
            x = right(x);
    return iterator(y);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>

inline pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
            typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const Key& k) {
    return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
}


template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>

typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const Key& k){
    pair<iterator, iterator> p = equal_range(k);
    size_type n = 0;
    distance(p.first, p.second, n);
    return n;
}

}
}
#endif
