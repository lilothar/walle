#ifndef WALLE_ALGO_SCOPED_PTR_H_
#define WALLE_ALGO_SCOPED_PTR_H_


#include <assert.h>
#include <walle/algo/checked_delete.h>
#include <memory>          // for std::auto_ptr

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

namespace std {

template<class T> class scoped_ptr // noncopyable
{
private:

    T * px;

    scoped_ptr(scoped_ptr const &);
    scoped_ptr & operator=(scoped_ptr const &);

    typedef scoped_ptr<T> this_type;

    void operator==( scoped_ptr const& ) const;
    void operator!=( scoped_ptr const& ) const;

public:

    typedef T element_type;

    explicit scoped_ptr( T * p = 0 ): px( p ) // never throws
    {
    }

    explicit scoped_ptr( std::auto_ptr<T> p ): px( p.release() ) // never throws
    {
    }

    ~scoped_ptr() // never throws
    {
        std::checked_delete( px );
    }

    void reset(T * p = 0) // never throws
    {
        assert( p == 0 || p != px ); // catch self-reset errors
        this_type(p).swap(*this);
    }

    T & operator*() const // never throws
    {
        assert( px != 0 );
        return *px;
    }

    T * operator->() const // never throws
    {
        assert( px != 0 );
        return px;
    }

    T * get() const // never throws
    {
        return px;
    }

    void swap(scoped_ptr & b) // never throws
    {
        T * tmp = b.px;
        b.px = px;
        px = tmp;
    }
    
    typedef T * this_type::*unspecified_bool_type;
    
       operator unspecified_bool_type() const // never throws
       {
           return px == 0? 0: &this_type::px;
       }
    
       // operator! is redundant, but some compilers need it
       bool operator! () const // never throws
       {
           return px == 0;
       }
};


template<class T> 
inline void swap(scoped_ptr<T> & a, scoped_ptr<T> & b) // never throws
{
    a.swap(b);
}

// get_pointer(p) is a generic way to say p.get()

template<class T> inline T * get_pointer(scoped_ptr<T> const & p)
{
    return p.get();
}

}


#endif 

