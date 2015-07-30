#ifndef WALLE_ALGO_H_
#define WALLE_ALGO_H_
#include "iterator.h"
namespace walle {
namespace algorithm {


template<typename InputIterator, typename Distance>
inline void distance(InputIterator first, InputIterator last, Distance& n) {
    for (; first != last; ++first)
        ++n;
}

template <class InputIterator, class Distance>
inline void advance(InputIterator& i, Distance n) 
{
    while(n--) ++i;
}


template<class ForwardIterator, class T>
inline ForwardIterator lower_bound(ForwardIterator first,
                                   ForwardIterator last,
                                   const T& value) 
 {
    typename iterator_traits<ForwardIterator>::difference_type len = 0;
    distance(first, last, len);
    typename iterator_traits<ForwardIterator>::difference_type half;
    ForwardIterator middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (*middle < value) {
            first = middle;
            ++first;
            len = len - half - 1;
        }
        else {
            len = half;
        }
    }
    return first;
}

template<class T>
inline void swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

}
}
#endif
