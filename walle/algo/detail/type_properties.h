#ifndef WALLE_STL_DETAIL_TYPE_PROPERTIES_H_
#define WALLE_STL_DETAIL_TYPE_PROPERTIES_H_


#include <limits.h>
//#include <walle/algo/detail/config.h>

namespace std
{

    template <typename T> struct is_const_value                    : public false_type{};
    template <typename T> struct is_const_value<const T*>          : public true_type{};
    template <typename T> struct is_const_value<const volatile T*> : public true_type{};

    template <typename T> struct is_const : public is_const_value<T*>{};
    template <typename T> struct is_const<T&> : public false_type{}; 
    template <typename T> struct is_volatile_value                    : public false_type{};
    template <typename T> struct is_volatile_value<volatile T*>       : public true_type{};
    template <typename T> struct is_volatile_value<const volatile T*> : public true_type{};

    template <typename T> struct is_volatile : public is_volatile_value<T*>{};
    template <typename T> struct is_volatile<T&> : public false_type{}; 


    template <typename T> struct is_signed : public false_type{};
    
    #define WALLESTL_TMP_DECLARE_SIGNED_WITH_CV(T)\
        template <> struct is_signed<T>                : public true_type{};\
        template <> struct is_signed<T const>          : public true_type{};\
        template <> struct is_signed<T volatile>       : public true_type{};\
        template <> struct is_signed<T const volatile> : public true_type{};
    
    WALLESTL_TMP_DECLARE_SIGNED_WITH_CV(signed char)
    WALLESTL_TMP_DECLARE_SIGNED_WITH_CV(signed short)
    WALLESTL_TMP_DECLARE_SIGNED_WITH_CV(signed int)
    WALLESTL_TMP_DECLARE_SIGNED_WITH_CV(signed long)
    WALLESTL_TMP_DECLARE_SIGNED_WITH_CV(signed long long)
    
    #if (CHAR_MAX == SCHAR_MAX)
        WALLESTL_TMP_DECLARE_SIGNED_WITH_CV(char)
    #endif
    
    #undef WALLESTL_TMP_DECLARE_SIGNED_WITH_CV
    
    #define WALLLESTL_DECLARE_SIGNED(T) namespace wallestl{ template <> struct is_signed<T> : public true_type{}; template <> struct is_signed<const T> : public true_type{}; }

    template <typename T> struct is_unsigned : public false_type{};
    
    #define WALLESTL_TMP_DECLARE_UNSIGNED_WITH_CV(T)\
        template <> struct is_unsigned<T>                : public true_type{};\
        template <> struct is_unsigned<T const>          : public true_type{};\
        template <> struct is_unsigned<T volatile>       : public true_type{};\
        template <> struct is_unsigned<T const volatile> : public true_type{};
    
    
    WALLESTL_TMP_DECLARE_UNSIGNED_WITH_CV(unsigned char)
    WALLESTL_TMP_DECLARE_UNSIGNED_WITH_CV(unsigned short)
    WALLESTL_TMP_DECLARE_UNSIGNED_WITH_CV(unsigned int)
    WALLESTL_TMP_DECLARE_UNSIGNED_WITH_CV(unsigned long)
    WALLESTL_TMP_DECLARE_UNSIGNED_WITH_CV(unsigned long long)
    
    #if (CHAR_MAX == UCHAR_MAX)
        WALLESTL_TMP_DECLARE_UNSIGNED_WITH_CV(char)
    #endif
    
    #undef WALLESTL_TMP_DECLARE_UNSIGNED_WITH_CV
    
    #define WALLESTL_DECLARE_UNSIGNED(T) namespace wallestl{ template <> struct is_unsigned<T> : public true_type{}; template <> struct is_unsigned<const T> : public true_type{}; }

    template <typename T>
    struct alignment_of_value{ static const size_t value = WALLESTL_ALIGN_OF(T); };

    template <typename T>
    struct alignment_of : public integral_constant<size_t, alignment_of_value<T>::value>{};

    template <typename T>
    struct is_aligned_value{ static const bool value = (WALLESTL_ALIGN_OF(T) > 8); };

    template <typename T> 
    struct is_aligned : public integral_constant<bool, is_aligned_value<T>::value>{};
} 


#endif // Header include guard




















