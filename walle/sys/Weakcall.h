#ifndef WALLE_SYS_WEAK_CALL_H_
#define WALLE_SYS_WEAK_CALL_H_
#include <functional>
#include <walle/algo/functional.h>
#include <walle/algo/memory.h>

namespace walle {
namespace sys{


template<typename CLASS>
class WeakCallback
{
 public:

  WeakCallback(const std::weak_ptr<CLASS>& object,
               const std::function<void (CLASS*)>& function)
    : object_(object), function_(function)
  {
  }

  // Default dtor, copy ctor and assignment are okay

  void operator()() const
  {
    std::shared_ptr<CLASS> ptr(object_.lock());
    if (ptr)
    {
      function_(ptr.get());
    }
  }

 private:

  std::weak_ptr<CLASS> object_;
  std::function<void (CLASS*)> function_;
};

template<typename CLASS>
WeakCallback<CLASS> makeWeakCallback(const std::shared_ptr<CLASS>& object,
                                     void (CLASS::*function)())
{
  return WeakCallback<CLASS>(object, function);
}

template<typename CLASS>
WeakCallback<CLASS> makeWeakCallback(const std::shared_ptr<CLASS>& object,
                                     void (CLASS::*function)() const)
{
  return WeakCallback<CLASS>(object, function);
}

}
}

#endif
