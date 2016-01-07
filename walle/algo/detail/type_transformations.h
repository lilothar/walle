#ifndef WALLE_STL_DETAIL_TYPE_TRANFORMATIONS_H_
#define WALLE_STL_DETAIL_TYPE_TRANFORMATIONS_H_


namespace std
{

    template<class T>
    struct add_signed
    { typedef T type; };

    template<>
    struct add_signed<unsigned char>
    { typedef signed char type; };

    #if (defined(CHAR_MAX) && defined(UCHAR_MAX) && (CHAR_MAX == UCHAR_MAX)) 
        struct add_signed<char>
        { typedef signed char type; };
    #endif

    template<>
    struct add_signed<unsigned short>
    { typedef short type; };

    template<>
    struct add_signed<unsigned int>
    { typedef int type; };

    template<>
    struct add_signed<unsigned long>
    { typedef long type; };

    template<>
    struct add_signed<unsigned long long>
    { typedef long long type; };

    template<class T>
    struct add_unsigned
    { typedef T type; };

    template<>
    struct add_unsigned<signed char>
    { typedef unsigned char type; };

    #if (defined(CHAR_MAX) && defined(SCHAR_MAX) && (CHAR_MAX == SCHAR_MAX)) // If char is signed (which is usually so)...
        template<>
        struct add_unsigned<char>
        { typedef unsigned char type; };
    #endif

    template<>
    struct add_unsigned<short>
    { typedef unsigned short type; };

    template<>
    struct add_unsigned<int>
    { typedef unsigned int type; };

    template<>
    struct add_unsigned<long>
    { typedef unsigned long type; };

    template<>
    struct add_unsigned<long long>
    { typedef unsigned long long type; };

    template <typename T> struct remove_cv_imp{};
    template <typename T> struct remove_cv_imp<T*>                { typedef T unqualified_type; };
    template <typename T> struct remove_cv_imp<const T*>          { typedef T unqualified_type; };
    template <typename T> struct remove_cv_imp<volatile T*>       { typedef T unqualified_type; };
    template <typename T> struct remove_cv_imp<const volatile T*> { typedef T unqualified_type; };

    template <typename T> struct remove_cv{ typedef typename remove_cv_imp<T*>::unqualified_type type; };
    template <typename T> struct remove_cv<T&>{ typedef T& type; }; // References are automatically not const nor volatile. See section 8.3.2p1 of the C++ standard.

    template <typename T, size_t N> struct remove_cv<T const[N]>         { typedef T type[N]; };
    template <typename T, size_t N> struct remove_cv<T volatile[N]>      { typedef T type[N]; };
    template <typename T, size_t N> struct remove_cv<T const volatile[N]>{ typedef T type[N]; };

    template <typename T>
    struct add_reference_impl{ typedef T& type; };

    template <typename T>
    struct add_reference_impl<T&>{ typedef T& type; };

    template <>
    struct add_reference_impl<void>{ typedef void type; };

    template <typename T>
    struct add_reference { typedef typename add_reference_impl<T>::type type; };


} 


#endif 





















