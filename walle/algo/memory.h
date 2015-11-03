#ifndef WALLE_ALGO_MEMORY_H_
#define WALLE_ALGO_MEMORY_H_

#include <walle/config/config.h>

#ifndef STD_CXX11
        #include<tr1/memory>

    namespace std{
        using ::std::tr1::shared_ptr;
        using ::std::tr1::weak_ptr;
        using ::std::tr1::enable_shared_from_this;
    }
#else
        #include<memory>
#endif

#endif