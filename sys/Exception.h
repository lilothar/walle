#ifndef WALLE_EXCEPTION_H_
#define WALLE_EXCEPTION_H_
#include <exception>
#include <string>

namespace walle {
namespace sys {
class Exception:std::exception{
    public:
      Exception(){}
      virtual ~Exception()throw(){}
      virtual const char* what() const throw();
      virtual const std::string getErrorMsg() { return "";}
      virtual const std::string makeErrorLog();
      
   private:
     mutable std::string _what;
};
class SimpleException:public Exception {
    public:
        SimpleException():_line(-1){}
        SimpleException(const char *msg,
            const char* file = "",
            const int line = -1);
        virtual ~SimpleException()throw(){}
        virtual const std::string getErrorMsg();
        virtual const std::string makeErrorLog();
        void                setErrorMsg(const char* emsg) {_msg = emsg;}
        
    private:
        std::string _msg;
        std::string _file;
        int         _line;
};
#define WALLETHROW(msg)  do { walleThrowException((msg), __FILE__, __LINE__) }while(0)
inline void walleThrowException(const char* msg, const char *file, const int line)
{
	throw SimpleException(msg, file, line);
}

}
}
#endif
