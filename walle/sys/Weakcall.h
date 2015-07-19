#ifndef DYLIN_WEAK_CALL_H_
#define DYLIN_WEAK_CALL_H_
#include <functional>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace walle {
namespace sys{


template<typename CLASS>
class WeakCallback
{
 public:

  WeakCallback(const boost::weak_ptr<CLASS>& object,
               const boost::function<void (CLASS*)>& function)
    : object_(object), function_(function)
  {
  }

  // Default dtor, copy ctor and assignment are okay

  void operator()() const
  {
    boost::shared_ptr<CLASS> ptr(object_.lock());
    if (ptr)
    {
      function_(ptr.get());
    }
  }

 private:

  boost::weak_ptr<CLASS> object_;
  boost::function<void (CLASS*)> function_;
};

template<typename CLASS>
WeakCallback<CLASS> makeWeakCallback(const boost::shared_ptr<CLASS>& object,
                                     void (CLASS::*function)())
{
  return WeakCallback<CLASS>(object, function);
}

template<typename CLASS>
WeakCallback<CLASS> makeWeakCallback(const boost::shared_ptr<CLASS>& object,
                                     void (CLASS::*function)() const)
{
  return WeakCallback<CLASS>(object, function);
}

}
}

#endif
