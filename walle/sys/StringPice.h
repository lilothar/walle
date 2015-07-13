#ifndef DYLIN_SLICE_H_
#define DYLIN_SLICE_H_
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <string>

namespace walle{
namespace sys{

class StringPice {
 public:
  // Create an empty slice.
  StringPice() : _data(""), _size(0) { }

  // Create a slice that refers to d[0,n-1].
  StringPice(const char* d, size_t n) : _data(d), _size(n) { }

  // Create a slice that refers to the contents of "s"
  StringPice(const std::string& s) : _data(s.data()), _size(s.size()) { }

  // Create a slice that refers to s[0,strlen(s)-1]
  StringPice(const char* s) : _data(s), _size(strlen(s)) { }

  // Return a pointer to the beginning of the referenced data
  const char* data() const { return _data; }

  // Return the length (in bytes) of the referenced data
  size_t size() const { return _size; }

  // Return true iff the length of the referenced data is zero
  bool empty() const { return _size == 0; }

  // Return the ith byte in the referenced data.
  // REQUIRES: n < size()
  char operator[](size_t n) const {
    assert(n < size());
    return _data[n];
  }

  // Change this slice to refer to an empty array
  void clear() { _data = ""; _size = 0; }

  // Drop the first "n" bytes from this slice.
  void remove_prefix(size_t n) {
    assert(n <= size());
    _data += n;
    _size -= n;
  }

  // Return a string that contains the copy of the referenced data.
  std::string ToString() const { return std::string(_data, _size); }

  // Three-way comparison.  Returns value:
  //   <  0 iff "*this" <  "b",
  //   == 0 iff "*this" == "b",
  //   >  0 iff "*this" >  "b"
  int compare(const StringPice& b) const;

  // Return true iff "x" is a prefix of "*this"
  bool starts_with(const StringPice& x) const {
    return ((_size >= x._size) &&
            (memcmp(_data, x._data, x._size) == 0));
  }

 private:
  const char* _data;
  size_t _size;

  // Intentionally copyable
};

inline bool operator==(const StringPice& x, const StringPice& y) {
  return ((x.size() == y.size()) &&
          (memcmp(x.data(), y.data(), x.size()) == 0));
}

inline bool operator!=(const StringPice& x, const StringPice& y) {
  return !(x == y);
}

inline int StringPice::compare(const StringPice& b) const {
  const size_t min_len = (_size < b._size) ? _size : b._size;
  int r = memcmp(_data, b._data, min_len);
  if (r == 0) {
    if (_size < b._size) r = -1;
    else if (_size > b._size) r = +1;
  }
  return r;
}
inline
bool operator>(const StringPice &x, const StringPice &y){
	return x.compare(y) > 0;
}

inline
bool operator>=(const StringPice &x, const StringPice &y){
	return x.compare(y) >= 0;
}

inline
bool operator<(const StringPice &x, const StringPice &y){
	return x.compare(y) < 0;
}

inline
bool operator<=(const StringPice &x, const StringPice &y){
	return x.compare(y) <= 0;
}

}
}
#endif /* SLICE_H_ */
