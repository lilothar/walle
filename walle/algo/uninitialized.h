#ifndef WALLE_UNINITIALIZED_H_
#define WALLE_UNINITIALIZED_H_
namespace walle {
namespace algorithm {
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator uninitalized_fill_n(ForwardIterator first, Size n, const T& x) {
    for (; n > 0; --n, ++first)
        construct(&*first, x);
    return first;
}

template<typename ForwardIterator, typename T>
void uninitalized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
    for (; first != last; ++first)
        construct(&*first, x);
}
template<typename InputIterator, typename ForwardIterator>
inline ForwardIterator uninitalized_copy(InputIterator first,
                                         InputIterator last,
                                         ForwardIterator result) {
    for (; first != last; ++first, ++result)
        construct(&*result, *first);
    return result;
}

template<typename InputIterator, typename OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
    for (; first != last; ++first, ++result)
        *result = *first;
    return result;
}

template<typename InputIterator, typename OutputIterator>
inline OutputIterator copy_backward(InputIterator first,
                                    InputIterator last,
                                    OutputIterator result) {
    for (; first != last; --last, --result) {
        *(result-1) = *(last-1);
    }
    return result;
}

template<typename InputIterator, typename T>
void fill(InputIterator first, InputIterator last, const T& value) {
    for (; first != last; ++first)
        *first = value;
}

template<typename T>
T max(const T& a, const T& b) {
    return a > b ? a : b;
}

}
}
#endif