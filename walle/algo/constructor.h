#ifndef WALLE_CONSTRUCTOR_H_
#define WALLE_CONSTRUCTOR_H_

#include <new>

namespace walle {
namespace algorithm {

template<typename T, typename VALUE>
inline void construct(T* p, const VALUE& value) {
    new (p) T(value);
}

template<typename T>
inline void destory(T* p) {
    p->~T();
}

template<typename ForwardIterator>
inline void destory(ForwardIterator first, ForwardIterator last) {
    for (; first < last; ++first)
        destory(&*first);
}

}
}
#endif