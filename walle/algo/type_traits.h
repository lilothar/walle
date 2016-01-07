#ifndef WALLE_STL_TYPE_TRAITS_H_
#define WALLE_STL_TYPE_TRAITS_H_
#include <walle/config/config.h>
#ifndef STD_CXX11
//#include <walle/algo/detail/config.h>
#include <stddef.h>               



namespace std {

    template <typename T, T v>
    struct integral_constant
    {
        static const T value = v;
        typedef T value_type;
        typedef integral_constant<T, v> type;
    };

    typedef integral_constant<bool, true>  true_type;
    typedef integral_constant<bool, false> false_type;

    typedef char yes_type;                     
    struct       no_type { char padding[8]; }; 

    template <bool bCondition, class ConditionIsTrueType, class ConditionIsFalseType>
    struct type_select { typedef ConditionIsTrueType type; };

    template <typename ConditionIsTrueType, class ConditionIsFalseType>
    struct type_select<false, ConditionIsTrueType, ConditionIsFalseType> { typedef ConditionIsFalseType type; };

    template <bool b1, bool b2, bool b3 = false, bool b4 = false, bool b5 = false>
    struct type_or;

    template <bool b1, bool b2, bool b3, bool b4, bool b5>
    struct type_or { static const bool value = true; };

    template <> 
    struct type_or<false, false, false, false, false> { static const bool value = false; };


    template <bool b1, bool b2, bool b3 = true, bool b4 = true, bool b5 = true>
    struct type_and;

    template <bool b1, bool b2, bool b3, bool b4, bool b5>
    struct type_and{ static const bool value = false; };

    template <>
    struct type_and<true, true, true, true, true>{ static const bool value = true; };

    template <int b1, int b2>
    struct type_equal{ static const bool value = (b1 == b2); };


    template <int b1, int b2>
    struct type_not_equal{ static const bool value = (b1 != b2); };



    template <bool b>
    struct type_not{ static const bool value = true; };

    template <>
    struct type_not<true>{ static const bool value = false; };




    template <typename T>
    struct empty{ };


}

#include <walle/algo/detail/type_fundamental.h>
#include <walle/algo/detail/type_transformations.h>
#include <walle/algo/detail/type_properties.h>
#include <walle/algo/detail/type_compound.h>
#include <walle/algo/detail/type_pod.h>
#else 

#include <type_traits>
#endif

#endif 





















