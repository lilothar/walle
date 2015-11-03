#ifndef WALLE_ALGO_UNORDER_H_
#define WALLE_ALGO_UNORDER_H_


#include <eva/config/config.h>

#ifndef STD_CXX11
        #include<tr1/unorder_set>
        #include<tr1/unorder_map>
    namespace std{
        using ::std::tr1::unorder_set;
        using ::std::tr1::unorder_map;
        using ::std::tr1::unorder_multiset;
        using ::std::tr1::unorder_multimap;
    }
#else
        #include<unorder_set>
        #include<unorder_map>
#endif
#endif
