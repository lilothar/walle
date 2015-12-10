#ifndef WALLE_BUFFER_H_
#define WALLE_BUFFER_H_
#include <stddef.h>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <string.h>
#include <walle/sys/Encode.h>
#include <walle/sys/StringPice.h>

using std::string;
using walle::sys::StringPice;
namespace walle {
namespace net {    
class Buffer {
public:
    static const size_t kPrepend = 8;
    static const size_t kInitSize = 1024;
    
    explicit Buffer(size_t initSize = kInitSize)
        : _buffer(kPrepend + initSize),
          _readerIndex(kPrepend),
          _writerIndex(kPrepend)
    {
        assert(readableBytes() == 0);
        assert(writableBytes() == initSize);
        assert(prependableBytes() == kPrepend);
    }
    
   
    void swap(Buffer& rhs)
    {
        _buffer.swap(rhs._buffer);
        std::swap(_readerIndex, rhs._readerIndex);
        std::swap(_writerIndex, rhs._writerIndex);
    }
    
    size_t readableBytes() const
    { 
        return _writerIndex - _readerIndex;
    }

    size_t writableBytes() const
    { 
        return _buffer.size() - _writerIndex; 
    }
    
    size_t prependableBytes() const
    { 
        return _readerIndex; 
    }
    
    const char* peek() const
    {
        return begin() + _readerIndex;
    }

    const char* findString(const string &str, const char* start = NULL) const
    {
        if(start) {
            assert(peek() <= start );
            assert(start <= beginWrite() );
        } else {
            start = peek();
        }
        const char* find = std::search(start, beginWrite(), 
                                    str.c_str(), 
                                    str.c_str() + str.length());
        return find == beginWrite() ? NULL : find;
    }     

    const char* findString(const StringPice &str, const char* start = NULL) const
    {
        if(start) {
            assert(peek() <= start );
            assert(start <= beginWrite() );
        } else {
            start = peek();
        }
        const char* find = std::search(start, beginWrite(), 
                                    str.data(), 
                                    str.data() + str.size());
        return find == beginWrite() ? NULL : find;
    }  

    const char* findChar(const char &c, const char* start = NULL)
    {
        if (start) {
            assert(peek() <= start);
            assert(start <= beginWrite());
        } else {
            start = peek();
        }
        const void* find = memchr(start, c, beginWrite() - start);
        return static_cast<const char*>(find);
        
    }
      const char* findCRLF() const
      {
        const char* crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF+2);
        return crlf == beginWrite() ? NULL : crlf;
      }
    
      const char* findCRLF(const char* start) const
      {
        assert(peek() <= start);
        assert(start <= beginWrite());
        const char* crlf = std::search(start, beginWrite(), kCRLF, kCRLF+2);
        return crlf == beginWrite() ? NULL : crlf;
      }
      const char* findEOL() const
      {
        const void* eol = memchr(peek(), '\n', readableBytes());
        return static_cast<const char*>(eol);
      }
    
      const char* findEOL(const char* start) const
      {
        assert(peek() <= start);
        assert(start <= beginWrite());
        const void* eol = memchr(start, '\n', beginWrite() - start);
        return static_cast<const char*>(eol);
      }
    
      // retrieve returns void, to prevent
      // string str(retrieve(readableBytes()), readableBytes());
      // the evaluation of two functions are unspecified
      void retrieve(size_t len)
      {
        assert(len <= readableBytes());
        if (len < readableBytes())
        {
          _readerIndex += len;
        }
        else
        {
          retrieveAll();
        }
      }
    
      void retrieveUntil(const char* end)
      {
        assert(peek() <= end);
        assert(end <= beginWrite());
        retrieve(end - peek());
      }
    
      void retrieveInt64()
      {
        retrieve(sizeof(int64_t));    
      }
    
      void retrieveInt32()
      {
        retrieve(sizeof(int32_t));
      }
    
      void retrieveInt16()
      {
        retrieve(sizeof(int16_t));
      }
    
      void retrieveInt8()
      {
        retrieve(sizeof(int8_t));
      }
    
      void retrieveAll()
      {
        _readerIndex = kPrepend;
        _writerIndex = kPrepend;
      }
    
      string retrieveAllAsString()
      {
        return retrieveAsString(readableBytes());;
      }
    
      string retrieveAsString(size_t len)
      {
        assert(len <= readableBytes());
        string result(peek(), len);
        retrieve(len);
        return result;
      }
      StringPice toSlice() const
      {
        return StringPice(peek(), static_cast<int>(readableBytes()));
      }
    
      void append(const StringPice& str)
      {
        append(str.data(), str.size());
      }
    
      void append(const char* /*restrict*/ data, size_t len)
      {
        ensureWritableBytes(len);
        std::copy(data, data+len, beginWrite());
        hasWritten(len);
      }
    
      void append(const void* /*restrict*/ data, size_t len)
      {
        append(static_cast<const char*>(data), len);
      }
    
      void ensureWritableBytes(size_t len)
      {
        if (writableBytes() < len)
        {
          makeSpace(len);
        }
        assert(writableBytes() >= len);
      }
    
      char* beginWrite()
      { return begin() + _writerIndex; }
    
      const char* beginWrite() const
      { return begin() + _writerIndex; }
    
      void hasWritten(size_t len)
      {
        assert(len <= writableBytes());
        _writerIndex += len;
      }
    
      void unwrite(size_t len)
      {
        assert(len <= readableBytes());
        _writerIndex -= len;
      }
    
      ///
      /// Append int64_t using network endian
      ///
      void appendInt64(int64_t x)
      {
        int64_t be64 = walle::sys::hton64(x);
        append(&be64, sizeof be64);
      }
    
      ///
      /// Append int32_t using network endian
      ///
      void appendInt32(int32_t x)
      {
        int32_t be32 = walle::sys::hton32(x);
        append(&be32, sizeof be32);
      }
    
      void appendInt16(int16_t x)
      {
        int16_t be16 = walle::sys::hton16(x);
        append(&be16, sizeof be16);
      }
    
      void appendInt8(int8_t x)
      {
        append(&x, sizeof x);
      }
    
      ///
      /// Read int64_t from network endian
      ///
      /// Require: buf->readableBytes() >= sizeof(int32_t)
      int64_t readInt64()
      {
        int64_t result = peekInt64();
        retrieveInt64();
        return result;
      }
    
      ///
      /// Read int32_t from network endian
      ///
      /// Require: buf->readableBytes() >= sizeof(int32_t)
      int32_t readInt32()
      {
        int32_t result = peekInt32();
        retrieveInt32();
        return result;
      }
    
      int16_t readInt16()
      {
        int16_t result = peekInt16();
        retrieveInt16();
        return result;
      }
    
      int8_t readInt8()
      {
        int8_t result = peekInt8();
        retrieveInt8();
        return result;
      }
    
      ///
      /// Peek int64_t from network endian
      ///
      /// Require: buf->readableBytes() >= sizeof(int64_t)
      int64_t peekInt64() const
      {
        assert(readableBytes() >= sizeof(int64_t));
        int64_t be64 = 0;
        ::memcpy(&be64, peek(), sizeof be64);
        return walle::sys::ntoh64(be64);
      }
    
      ///
      /// Peek int32_t from network endian
      ///
      /// Require: buf->readableBytes() >= sizeof(int32_t)
      int32_t peekInt32() const
      {
        assert(readableBytes() >= sizeof(int32_t));
        int32_t be32 = 0;
        ::memcpy(&be32, peek(), sizeof be32);
        return walle::sys::ntoh32(be32);
      }
    
      int16_t peekInt16() const
      {
        assert(readableBytes() >= sizeof(int16_t));
        int16_t be16 = 0;
        ::memcpy(&be16, peek(), sizeof be16);
        return walle::sys::ntoh16(be16);
      }
    
      int8_t peekInt8() const
      {
        assert(readableBytes() >= sizeof(int8_t));
        int8_t x = *peek();
        return x;
      }
    
      ///
      /// Prepend int64_t using network endian
      ///
      void prependInt64(int64_t x)
      {
        int64_t be64 = walle::sys::hton64(x);
        prepend(&be64, sizeof be64);
      }
    
      ///
      /// Prepend int32_t using network endian
      ///
      void prependInt32(int32_t x)
      {
        int32_t be32 = walle::sys::hton32(x);
        prepend(&be32, sizeof be32);
      }
    
      void prependInt16(int16_t x)
      {
        int16_t be16 = walle::sys::hton16(x);
        prepend(&be16, sizeof be16);
      }
    
      void prependInt8(int8_t x)
      {
        prepend(&x, sizeof x);
      }
    
      void prepend(const void* data, size_t len)
      {
        assert(len <= prependableBytes());
        _readerIndex -= len;
        const char* d = static_cast<const char*>(data);
        std::copy(d, d+len, begin()+_readerIndex);
      }
    
      void shrink(size_t reserve)
      {
        // FIXME: use vector::shrink_to_fit() in C++ 11 if possible.
        Buffer other;
        other.ensureWritableBytes(readableBytes()+reserve);
        other.append(toSlice());
        swap(other);
      }
    
      size_t Capacity() const
      {
        return _buffer.capacity();
      }
    
      /// Read data directly into buffer.
      ///
      /// It may implement with readv(2)
      /// @return result of read(2), @c errno is saved
      ssize_t readFd(int fd, int* savedErrno);
    
     private:
    
      char* begin()
      { return &*_buffer.begin(); }
    
      const char* begin() const
      { return &*_buffer.begin(); }
    
      void makeSpace(size_t len)
      {
        if (writableBytes() + prependableBytes() < len + kPrepend)
        {
          // FIXME: move readable data
          _buffer.resize(_writerIndex+len);
        }
        else
        {
          // move readable data to the front, make space inside buffer
          assert(kPrepend < _readerIndex);
          size_t readable = readableBytes();
          std::copy(begin()+_readerIndex,
                    begin()+_writerIndex,
                    begin()+kPrepend);
          _readerIndex = kPrepend;
          _writerIndex = _readerIndex + readable;
          assert(readable == readableBytes());
        }
      }
    
     private:
      std::vector<char> _buffer;
      size_t _readerIndex;
      size_t _writerIndex;
    
      static const char kCRLF[];
    };

}

}
#endif
