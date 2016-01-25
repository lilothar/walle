#ifndef WALLE_HTTPUTILS_H_
#define WALLE_HTTPUTILS_H_

#include <string>
#include <algorithm>
#include <stddef.h>

#define DEFAULT_MASK_VALUE 0xFFFF

namespace walle {
namespace http {


#define COMPARATOR(x, y, op) \
    { \
        size_t l1 = (x).size();\
        size_t l2 = (y).size();\
        if (l1 < l2) return true;\
        if (l1 > l2) return false;\
        \
        for (size_t n = 0; n < l1; n++)\
        {\
            int xc = op((x)[n]);\
            int yc = op((y)[n]);\
            if (xc < yc) return true;\
            if (xc > yc) return false;\
        }\
        return false;\
    }

class header_comparator {
    public:
        /**
         * Operator used to compare strings.
         * @param first string
         * @param second string
        **/
        bool operator()(const std::string& x,const std::string& y) const
        {
            COMPARATOR(x, y, std::toupper);
        }
};

/**
 * Operator Class that is used to compare two strings. The comparison can be sensitive or insensitive.
 * The default comparison is case sensitive. To obtain insensitive comparison you have to pass in
 * compilation phase the flag CASE_INSENSITIVE to the preprocessor.
**/
class arg_comparator {
    public:
        /**
         * Operator used to compare strings.
         * @param first string
         * @param second string
        **/
        bool operator()(const std::string& x,const std::string& y) const
        {
            COMPARATOR(x, y, std::toupper);
		}
};
}
}
#endif

