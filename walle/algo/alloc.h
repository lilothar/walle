#ifndef WALLE_ALLOCATOR_H_
#define WALLE_ALLOCATOR_H_
#include <stddef.h>
#include <stdlib.h>
namespace walle {
namespace algorithm {

class allocator{
	public:
	static void* allocate(size_t n)
	{
		void *result = malloc(n);
		return result;
	}

	static void deallocate(void* p)
	{
		free(p);
	}
	
	static void deallocate(void* p, size_t n)
	{
		free(p);
	}
	static void* reallocate(void* p, size_t old, size_t nsize)
	{
		void *result = realloc(p, nsize);
		return result;
	}
};

template <typename T,typename Alloc> 
 class default_alloc{
	public:
		static T* allocate(size_t n)
		{
			return n == 0? (T*)0:(T*)Alloc::allocate(sizeof(T) * n);
		}

		static T* allocate(void)
		{
			return (T*)Alloc::allocate(sizeof(T));
		}

		static void deallocate(T* e, size_t n) 
		{
			if(n) {
				Alloc::deallocate(e, sizeof(T) * n);
			}
		}
		static void deallocate(T* e)
		{
			Alloc::deallocate(e, sizeof(T));
		}
};
}
}
#endif
