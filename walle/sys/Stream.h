#ifndef WALLE_STREAM_H_
#define WALLE_STREAM_H_
#include <stddef.h>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <string.h>
#include <walle/sys/StringPice.h>

using std::string;
using std::vector;

namespace walle {
namespace sys {    
class BytesStream {
public:
    static const size_t kInitSize = 1024;
    
    explicit BytesStream(size_t initSize = kInitSize)
        : _buffer(initSize),
          _readerIndex(0),
          _writerIndex(0)
    {
        assert(readableBytes() == 0);
        assert(writableBytes() == initSize);
    }

    
   BytesStream(const string& buf )
        :_buffer(buf.size()+kInitSize),
         _readerIndex(0),
         _writerIndex(buf.size())
   {
        std::copy(buf.c_str(),buf.c_str() + buf.size(),&*_buffer.begin());
   }

   BytesStream(const StringPice & pice)
        :_buffer(pice.size()+kInitSize),
         _readerIndex(0),
         _writerIndex(pice.size())
   {
        std::copy(pice.data(), pice.data() + pice.size(),&*_buffer.begin());
   }

   BytesStream(const char* str, size_t len)
        :_buffer(len+kInitSize),
         _readerIndex(0),
         _writerIndex(len)
    {
        std::copy(str, str + len, &*_buffer.begin());
    }
    
   
    size_t readableBytes() const
    { 
        return _writerIndex - _readerIndex;
    }

    size_t writableBytes() const
    { 
        return _buffer.size() - _writerIndex; 
    }
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

    void retrieveAll()
    {
        _readerIndex = 0;
        _writerIndex = 0;
    }
    
    
    const char* peek() const
    {
        return begin() + _readerIndex;
    }

    size_t read(char* buff, size_t count) 
    {
        size_t readable =readableBytes();
        if(count <readable) {
            
            memcpy(buff, peek(), readable);
            retrieveAll();
            return readable;
        }
        memcpy(buff, peek(), count);
        retrieve(count);
        return count;      
    }

    size_t readLine(char* buff,size_t count) 
    {
        
        const char* eol =findEOL();
        size_t readable =readableBytes();
        if(!eol) {
           if(count < readable)  {
                memcpy(buff,peek(),count);
                retrieve(count);
                return count;
           } else {
                memcpy(buff,peek(),readable);
                retrieve(readable);
                return readable;  
           }
        }
        size_t linelen = eol+1 - peek();

        if( linelen > count) {
            memcpy(buff,peek(),count);
            retrieve(count);
            return count;   
        } else {
            memcpy(buff,peek(),linelen);
            retrieve(linelen);
            return linelen;    
        }
    }

    size_t readLine(string &result)
    {
        const char* eol =findEOL();
        result.clear();
        if(!eol) {
            result.append(peek(),readableBytes());
            retrieveAll();
            return result.size();
        }

        result.append(peek(),eol+1 - peek());
        retrieve(eol+1 - peek());
        return result.size();
        
    }
    const char* findEOL() const
    {
        const void* eol = memchr(peek(), '\n', readableBytes());
        return static_cast<const char*>(eol);
    }
    void appendLine(const char* /*restrict*/ data, size_t len)
    {
        ensureWritableBytes(len);
        std::copy(data, data+len, beginWrite());   
        hasWritten(len);   
        const char * eol = "\n";
        std::copy(eol, eol+1, beginWrite());
        hasWritten(1); 
     }

     void appendLine(const string &str)
     {
        appendLine(str.c_str(), str.size());
     }

     void appendLine(const StringPice&str)
     {
        appendLine(str.data(), str.size());
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

     void append(const string &str) 
     {
        append(str.c_str(), str.size());  
     }

     void append(const StringPice&str) 
     {
        append(str.data(), str.size());  
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
     void swap(BytesStream& rhs)
    {
        _buffer.swap(rhs._buffer);
        std::swap(_readerIndex, rhs._readerIndex);
        std::swap(_writerIndex, rhs._writerIndex);
    }
     
     StringPice toSlice() const
     {
       return StringPice(peek(), static_cast<int>(readableBytes()));
     }
    
      void shrink(size_t reserve)
      {
        BytesStream other;
        other.ensureWritableBytes(readableBytes()+reserve);
        other.append(toSlice());
        swap(other);
      }
    
      size_t Capacity() const
      {
        return _buffer.capacity();
      }
    
    
     private:
    
      char* begin()
      { return &*_buffer.begin(); }
    
      const char* begin() const
      { return &*_buffer.begin(); }
    
      void makeSpace(size_t len)
      {
        if (writableBytes()  < len)
        {
          // FIXME: move readable data
          _buffer.resize(_writerIndex+len);
        }
        else
        {
          // move readable data to the front, make space inside buffer
   
          size_t readable = readableBytes();
          std::copy(begin()+_readerIndex,
                    begin()+_writerIndex,
                    begin());
          _readerIndex = 0;
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
