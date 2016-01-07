#ifndef WALLE_STL_INTERNAL_TYPE_FUNDAMENTAL_H_
#define WALLE_STL_INTERNAL_TYPE_FUNDAMENTAL_H_


namespace std {

    template <typename T> struct is_void : public false_type{};

    template <> struct is_void<void> : public true_type{};
    template <> struct is_void<void const> : public true_type{};
    template <> struct is_void<void volatile> : public true_type{};
    template <> struct is_void<void const volatile> : public true_type{};


    ///////////////////////////////////////////////////////////////////////
    // is_integral
    //
    // is_integral<T>::value == true if and only if T  is one of the following types:
    //    [const] [volatile] bool
    //    [const] [volatile] char
    //    [const] [volatile] signed char
    //    [const] [volatile] unsigned char
    //    [const] [volatile] wchar_t
    //    [const] [volatile] short
    //    [const] [volatile] int
    //    [const] [volatile] long
    //    [const] [volatile] long long
    //    [const] [volatile] unsigned short
    //    [const] [volatile] unsigned int
    //    [const] [volatile] unsigned long
    //    [const] [volatile] unsigned long long
    //
    ///////////////////////////////////////////////////////////////////////
    template <typename T> struct is_integral : public false_type{};
    
    #define WALLESTL_TMP_DECLARE_INTEGRAL_WITH_CV(T)\
        template <> struct is_integral<T>                : public true_type{};\
        template <> struct is_integral<T const>          : public true_type{};\
        template <> struct is_integral<T volatile>       : public true_type{};\
        template <> struct is_integral<T const volatile> : public true_type{};
    
    WALLESTL_TMP_DECLARE_INTEGRAL_WITH_CV(unsigned char)
    WALLESTL_TMP_DECLARE_INTEGRAL_WITH_CV(unsigned short)
    WALLESTL_TMP_DECLARE_INTEGRAL_WITH_CV(unsigned int)
    WALLESTL_TMP_DECLARE_INTEGRAL_WITH_CV(unsigned long)
    WALLESTL_TMP_DECLARE_INTEGRAL_WITH_CV(unsigned long long)
    
    WALLESTL_TMP_DECLARE_INTEGRAL_WITH_CV(signed char)
    WALLESTL_TMP_DECLARE_INTEGRAL_WITH_CV(signed short)
    WALLESTL_TMP_DECLARE_INTEGRAL_WITH_CV(signed int)
    WALLESTL_TMP_DECLARE_INTEGRAL_WITH_CV(signed long)
    WALLESTL_TMP_DECLARE_INTEGRAL_WITH_CV(signed long long)
    
    WALLESTL_TMP_DECLARE_INTEGRAL_WITH_CV(bool)
    WALLESTL_TMP_DECLARE_INTEGRAL_WITH_CV(char)
    
    
    #undef WALLESTL_TMP_DECLARE_INTEGRAL_WITH_CV


    ///////////////////////////////////////////////////////////////////////
    // is_floating_point
    //
    // is_floating_point<T>::value == true if and only if T is one of the following types:
    //    [const] [volatile] float
    //    [const] [volatile] double
    //    [const] [volatile] long double
    //
    ///////////////////////////////////////////////////////////////////////
    template <typename T> struct is_floating_point : public false_type{};
    
    #define WALLESTL_TMP_DECLARE_FLOATING_POINT_WITH_CV(T)\
        template <> struct is_floating_point<T>                : public true_type{};\
        template <> struct is_floating_point<T const>          : public true_type{};\
        template <> struct is_floating_point<T volatile>       : public true_type{};\
        template <> struct is_floating_point<T const volatile> : public true_type{};
    
    WALLESTL_TMP_DECLARE_FLOATING_POINT_WITH_CV(float)
    WALLESTL_TMP_DECLARE_FLOATING_POINT_WITH_CV(double)
    WALLESTL_TMP_DECLARE_FLOATING_POINT_WITH_CV(long double)
    
    #undef WALLESTL_TMP_DECLARE_FLOATING_POINT_WITH_CV


    ///////////////////////////////////////////////////////////////////////
    // is_arithmetic
    //
    // is_arithmetic<T>::value == true if and only if:
    //    is_floating_point<T>::value == true, or
    //    is_integral<T>::value == true
    //
    ///////////////////////////////////////////////////////////////////////
    template <typename T> 
    struct is_arithmetic : public integral_constant<bool,
        is_integral<T>::value || is_floating_point<T>::value
    >{};


    ///////////////////////////////////////////////////////////////////////
    // is_fundamental
    //
    // is_fundamental<T>::value == true if and only if:
    //    is_floating_point<T>::value == true, or
    //    is_integral<T>::value == true, or
    //    is_void<T>::value == true
    ///////////////////////////////////////////////////////////////////////
    template <typename T> 
    struct is_fundamental : public integral_constant<bool,
        is_void<T>::value || is_integral<T>::value || is_floating_point<T>::value
    >{};

} 

#endif 




















