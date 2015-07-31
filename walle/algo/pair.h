#ifndef WALLE_PAIR_H__
#define WALLE_PAIR_H__

namespace walle {
namespace algorithm {

template<class T1, class T2>
struct pair {
    typedef T1 first_type;
    typedef T1 second_type;
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) {}
    pair(const T1& a, const T2& b) : first(a), second(b){ }
    template<class U1, class U2>
    pair(const pair<U1, U2>& p) : first(p.first), second(p.second){}
};

template<class T1, class T2>
inline bool operator==(const pair<T1, T2>& a, const pair<T1, T2>& b) {
    return a.first == b.first && a.second == b.second;
}

template<class T1, class T2>
inline bool operator<(const pair<T1, T2>& a, const pair<T1, T2>& b) {
    return a.first < b.first || (!(b.first < a.first) && a.second < b.second);
}

template<class T1, class T2>
inline pair<T1, T2> make_pair(const T1& x, const T2& y) {
    return pair<T1, T2>(x, y);
}
}
}
#endif
