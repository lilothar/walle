#ifndef WALLE_LOGSTREAM_H_
#define WALLE_LOGSTREAM_H_
#include <walle/sys/Fixbuffer.h>
#include <walle/sys/StringUtil.h>
#include <walle/sys/StringPice.h>

namespace walle {

namespace sys {
    
class LogStream{
public:
    typedef LogStream self;
    typedef FixBuffer<ksmallbuffer> Buffer;
public:
    LogStream(bool w = true):_write(w){}
    ~LogStream(){}
    self& operator<<(bool v)
    {
        if(_write) {
            _buffer.append(v ? "1" : "0", 1);
        }
        return *this;
    }

    self& operator<<(short);
    self& operator<<(unsigned short);
    self& operator<<(int);
    self& operator<<(unsigned int);
    self& operator<<(long);
    self& operator<<(unsigned long);
    self& operator<<(long long);
    self& operator<<(unsigned long long);
    //self& operator<<(const Console::TextColor& color);
    self& operator<<(const void*);

    self& operator<<(float v)
    {
        if(_write) {
            *this << static_cast<double>(v);
        }
        return *this;
    }

    self& operator<<(double);
  // self& operator<<(long double);

  self& operator<<(char v)
  {
    if(_write) {
        _buffer.append(&v, 1);
    }
    return *this;
  }

  self& operator<<(const char* v)
  {
    if(_write) {
        _buffer.append(v, strlen(v));
    }
    return *this;
  }

  self& operator<<(const string& v)
  {
    if(_write) {
        _buffer.append(v.c_str(), v.size());
    }
    return *this;
  }

  self& operator<<(const StringPice& v)
  {
    if(_write) {
        _buffer.append(v.data(), v.size());
    }
    return *this;
  }

  void append(const char* data, size_t len)
  {
    if(_write) {
        _buffer.append(data, len);
    }
  }
  const Buffer& buffer() const { return _buffer; }
  void resetBuffer() { _buffer.reset(); }
private:
    template <typename T>
    void appendInt(T value) 
    {
        string result = StringUtil::formateInteger(value);
        if(_buffer.avail() > result.length()) {
            _buffer.append(result.c_str(),result.length());
        }
        
    }
    Buffer _buffer;
    bool   _write;

};
}
}
#endif
