#ifndef WALLE_ALGO_FUNCTIONAL_H_
#define WALLE_ALGO_FUNCTIONAL_H_
#include <walle/config/config.h>

#ifndef STD_CXX11
    #include <tr1/functional>
    namespace std{
        using std::tr1::function;
        using std::tr1::bind;
        using std::tr1::placeholders::_1;
        using std::tr1::placeholders::_2;
        using std::tr1::placeholders::_3;
        using std::tr1::placeholders::_4;
        using std::tr1::placeholders::_5;
        using std::tr1::placeholders::_6;
        using std::tr1::placeholders::_7;
        using std::tr1::placeholders::_8;
        
    }

#else 
    #include <functional>
    namespace std{
    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    using std::placeholders::_4;
    using std::placeholders::_5;
    using std::placeholders::_6;
    using std::placeholders::_7;
    using std::placeholders::_8;
         
    }
#endif

#endif
