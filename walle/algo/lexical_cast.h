#ifndef WALLE_ALGO_LEXICAL_CAST_H_
#define WALLE_ALGO_LEXICAL_CAST_H_
#include <sstream>
namespace std { 


template <typename Target, typename Source, bool Same>
class lexical_cast_t{
public:
  static Target cast(const Source &arg){
    Target ret;
    std::stringstream ss;
    if (!(ss<<arg && ss>>ret && ss.eof()))
      throw std::bad_cast();
    
    return ret;
  }
};

template <typename Target, typename Source>
class lexical_cast_t<Target, Source, true>{
public:
  static Target cast(const Source &arg){
    return arg;
  }  
};

template <typename Source>
class lexical_cast_t<std::string, Source, false>{
public:
  static std::string cast(const Source &arg){
    std::ostringstream ss;
    ss<<arg;
    return ss.str();
  }
};

template <typename Target>
class lexical_cast_t<Target, std::string, false>{
public:
  static Target cast(const std::string &arg){
    Target ret;
    std::istringstream ss(arg);
    if (!(ss>>ret && ss.eof()))
      throw std::bad_cast();
    return ret;
  }
};


template<typename Target, typename Source>
Target lexical_cast(const Source &arg)
{
  return lexical_cast_t<Target, Source, is_same<Target, Source>::value>::cast(arg);
}

}
#endif
