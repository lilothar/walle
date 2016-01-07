#ifndef WALLE_ALGO_ONE_OF_H_
#define WALLE_ALGO_ONE_OF_H_

namespace std {

template <class T>
struct oneof_reader{
  T operator()(const std::string &s){
    T ret=default_reader<T>()(s);
    if (std::find(alt.begin(), alt.end(), ret)==alt.end())
      throw cmdline_error("");
    return ret;
  }
  void add(const T &v){ alt.push_back(v); }
private:
  std::vector<T> alt;
};

template <class T>
oneof_reader<T> oneof(T a1)
{
  oneof_reader<T> ret;
  ret.add(a1);
  return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2)
{
  oneof_reader<T> ret;
  ret.add(a1);
  ret.add(a2);
  return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3)
{
  oneof_reader<T> ret;
  ret.add(a1);
  ret.add(a2);
  ret.add(a3);
  return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4)
{
  oneof_reader<T> ret;
  ret.add(a1);
  ret.add(a2);
  ret.add(a3);
  ret.add(a4);
  return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5)
{
  oneof_reader<T> ret;
  ret.add(a1);
  ret.add(a2);
  ret.add(a3);
  ret.add(a4);
  ret.add(a5);
  return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6)
{
  oneof_reader<T> ret;
  ret.add(a1);
  ret.add(a2);
  ret.add(a3);
  ret.add(a4);
  ret.add(a5);
  ret.add(a6);
  return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7)
{
  oneof_reader<T> ret;
  ret.add(a1);
  ret.add(a2);
  ret.add(a3);
  ret.add(a4);
  ret.add(a5);
  ret.add(a6);
  ret.add(a7);
  return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8)
{
  oneof_reader<T> ret;
  ret.add(a1);
  ret.add(a2);
  ret.add(a3);
  ret.add(a4);
  ret.add(a5);
  ret.add(a6);
  ret.add(a7);
  ret.add(a8);
  return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8, T a9)
{
  oneof_reader<T> ret;
  ret.add(a1);
  ret.add(a2);
  ret.add(a3);
  ret.add(a4);
  ret.add(a5);
  ret.add(a6);
  ret.add(a7);
  ret.add(a8);
  ret.add(a9);
  return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8, T a9, T a10)
{
  oneof_reader<T> ret;
  ret.add(a1);
  ret.add(a2);
  ret.add(a3);
  ret.add(a4);
  ret.add(a5);
  ret.add(a6);
  ret.add(a7);
  ret.add(a8);
  ret.add(a9);
  ret.add(a10);
  return ret;
}

}

#endif
