#ifndef WALLE_ALGO_CHECKED_DELETE_H_
#define WALLE_ALGO_CHECKED_DELETE_H_

namespace std {

template<class T> 
inline void checked_delete(T * x)
{
	typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
	(void) sizeof(type_must_be_complete);
	delete x;
}

template <class T> 
inline void checked_array_delete(T *x)
{
	typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
	(void) sizeof(type_must_be_complete);
	delete [] x;
}

template<class T> 
struct checked_deleter
{
	typedef void result_type;
	typedef T * argument_type;

	void operator()(T * x) const
	{
		checked_delete(x);
	}
};

template<class T> 
struct checked_array_deleter
{
    typedef void result_type;
	typedef T * argument_type;

	void operator()(T * x) const
	{
		checked_array_delete(x);
	}
};
}

#endif