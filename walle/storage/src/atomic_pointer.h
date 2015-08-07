#ifndef PORT_ATOMIC_POINTER_H_
#define PORT_ATOMIC_POINTER_H_

#include <stdint.h>

#define ARCH_CPU_X86_FAMILY 1

namespace walle {
namespace storage {


inline void MemoryBarrier() {
  // See http://gcc.gnu.org/ml/gcc/2003-04/msg01180.html for a discussion on
  // this idiom. Also see http://en.wikipedia.org/wiki/Memory_ordering.
  __asm__ __volatile__("" : : : "memory");
}
#define LEVELDB_HAVE_MEMORY_BARRIER
class AtomicPointer {
 private:
  void* rep_;
 public:
  AtomicPointer() { }
  explicit AtomicPointer(void* p) : rep_(p) {}
  inline void* NoBarrier_Load() const { return rep_; }
  inline void NoBarrier_Store(void* v) { rep_ = v; }
  inline void* Acquire_Load() const {
    void* result = rep_;
    MemoryBarrier();
    return result;
  }
  inline void Release_Store(void* v) {
    MemoryBarrier();
    rep_ = v;
  }
};
#undef LEVELDB_HAVE_MEMORY_BARRIER
#undef ARCH_CPU_X86_FAMILY

}  
}  
#endif  // PORT_ATOMIC_POINTER_H_
