#ifndef WALLE_STL_DETAIL_CONFIG_H_
#define WALLE_STL_DETAIL_CONFIG_H_

#include <walle/config/config.h>
#include <assert.h>

#define WALLESTL_ALIGN_OF(type) ((size_t)__alignof__(type))
#define WALLESTL_DEFAULT_NAME_PREFIX "WSTL_ALLOCATOR"

#ifdef WALLE_DEBUG
# define WALLESTL_NAME_ENABLED 1
#else 
# define WALLESTL_NAME_ENABLED 0
#endif


#ifndef WALLESTLAllocatorType
# define WALLESTLAllocatorType wallestl::allocator
#endif

#ifndef WALLESTLAllocatorDefault
# define WALLESTLAllocatorDefault wallestl::GetDefaultAllocator
#endif

#ifndef WALLESTLFree
# define WALLESTLFree(allocator, p, size) (allocator).deallocate((p), (size))
#endif

#ifndef WALLESTLAlloc
#define WALLESTLAlloc(allocator, n) (allocator).allocate(n);
#endif

#ifndef WALLESTLAllocAligned
#define WALLESTLAllocAligned(allocator, n, alignment, offset) (allocator).allocate((n), (alignment), (offset))
#endif

#ifndef WALLESTLAllocFlags
#define WALLESTLAllocFlags(allocator, n, flags) (allocator).allocate(n, flags);
#endif

#define WALLE_FORCE_INLINE inline __attribute__((always_inline))


#define WALLESTL_LIKELY(x)   __builtin_expect(!!(x), true)
#define WALLESTL_UNLIKELY(x)   __builtin_expect(!!(x), false)


#define WALLESTL_MAX_STACK_USAGE 4000

#endif
