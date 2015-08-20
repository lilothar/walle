#ifndef WALLE_WLOG_WLOG_H_
#define WALLE_WLOG_WLOG_H_
namespace walle {
namespace wlog{

class Category;

class Wlog{
    public:
    enum LogLevel{
        DEBUG = 20,
        TRACE = 40,
        INFO  = 60,
        NOTICE = 80,
        WARN  = 100,
        ERROR = 120,
        FATAL = 140
    };
    
    class SourceFile
      {
       public:
        template<int N>
        inline SourceFile(const char (&arr)[N])
          : _data(arr),
            _size(N-1)
        {
          const char* slash = strrchr(_data, '/'); 
          if (slash)
          {
            _data = slash + 1;
            _size -= _data - arr;
          }
        }
    
        explicit SourceFile(const char* filename)
          : _data(filename)
        {
          const char* slash = strrchr(filename, '/');
          if (slash)
          {
            _data = slash + 1;
          }
          _size = static_cast<int>(strlen(_data));
        }
    
        const char* _data;
        size_t      _size;
      };
    Wlog();
    ~Wlog();
    bool init(const string &confpath);
    bool reConfig(const string &confpath);
    static Category* getCategory(const string &cname);
    class Logger {
        public:
            Logger(const Category *c,SourceFile file, int line, LogLevel level, const char* func);


    };
    
   // Wlog(SourceFile file, int line, LogLevel level, const char* func);
};

}
}
#endif
