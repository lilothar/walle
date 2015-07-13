#ifndef WALLE_TYPES_H_
#define WALLE_TYPES_H_
namespace walle {
namespace sys {

template<typename To, typename From>
inline To implicit_cast(From const &f) 
{  
        return f;
}

}

}
#endif
