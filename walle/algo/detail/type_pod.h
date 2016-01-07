#ifndef WALLE_STL_DETAIL_TYPE_POD_H_
#define WALLE_STL_DETAIL_TYPE_POD_H_


#include <limits.h>


namespace std {
    template <typename T>
    struct is_empty_helper_t1 : public T { char m[64]; };
    struct is_empty_helper_t2            { char m[64]; };

    // The inheritance in empty_helper_t1 will not work with non-class types
    template <typename T, bool is_a_class = false>
    struct is_empty_helper : public false_type{};

    template <typename T>
    struct is_empty_helper<T, true> : public integral_constant<bool,
        sizeof(is_empty_helper_t1<T>) == sizeof(is_empty_helper_t2)
    >{};

    template <typename T>
    struct is_empty_helper2
    {
        typedef typename remove_cv<T>::type _T;
        typedef is_empty_helper<_T, is_class<_T>::value> type;
    };

    template <typename T> 
    struct is_empty : public is_empty_helper2<T>::type {};
    template <typename T> // There's not much we can do here without some compiler extension.
    struct is_pod : public integral_constant<bool, is_void<T>::value || is_scalar<T>::value>{};

    template <typename T, size_t N>
    struct is_pod<T[N]> : public is_pod<T>{};

    template <typename T>
    struct is_POD : public is_pod<T>{};

    #define WALLESTL_DECLARE_POD(T) namespace wallestl{ template <> struct is_pod<T> : public true_type{}; template <> struct is_pod<const T> : public true_type{}; }
    template <typename T> 
    struct has_trivial_constructor : public is_pod<T> {};

    #define WALLESTL_DECLARE_TRIVIAL_CONSTRUCTOR(T) namespace wallestl{ template <> struct has_trivial_constructor<T> : public true_type{}; template <> struct has_trivial_constructor<const T> : public true_type{}; }
    template <typename T> 
    struct has_trivial_copy : public integral_constant<bool, is_pod<T>::value && !is_volatile<T>::value>{};

    #define WALLESTL_DECLARE_TRIVIAL_COPY(T) namespace wallestl{ template <> struct has_trivial_copy<T> : public true_type{}; template <> struct has_trivial_copy<const T> : public true_type{}; }
    template <typename T> 
    struct has_trivial_assign : public integral_constant<bool,
        is_pod<T>::value && !is_const<T>::value && !is_volatile<T>::value
    >{};

    #define WALLESTL_DECLARE_TRIVIAL_ASSIGN(T) namespace wallestl{ template <> struct has_trivial_assign<T> : public true_type{}; template <> struct has_trivial_assign<const T> : public true_type{}; }

    template <typename T> 
    struct has_trivial_destructor : public is_pod<T>{};

    #define WALLESTL_DECLARE_TRIVIAL_DESTRUCTOR(T) namespace wallestl{ template <> struct has_trivial_destructor<T> : public true_type{}; template <> struct has_trivial_destructor<const T> : public true_type{}; }

    template <typename T> 
    struct has_trivial_relocate : public integral_constant<bool, is_pod<T>::value && !is_volatile<T>::value>{};

    #define WALLESTL_DECLARE_TRIVIAL_RELOCATE(T) namespace wallestl{ template <> struct has_trivial_relocate<T> : public true_type{}; template <> struct has_trivial_relocate<const T> : public true_type{}; }


} 


#endif // Header include guard






















