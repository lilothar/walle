#ifndef WALLE_FD_H_
#define WALLE_FD_H_

namespace walle{
namespace sys{
class IOComponet;
class EventFD{
    public:
        EventFD(){}
        virtual ~EventFD() {}
        virtual int read(char *data, int len) = 0;
        virtual int write(char* data, int len) = 0;
        virtual void close();
        IOComponet * getIoc() { return _ioc;}
        void setIoc(IOComponet *i) { _ioc = i; }
        void setFd(int fd) { _fd = fd; }
        bool valid() { return _fd > 0; }
        int getHandler() {return _fd; }
        
    protected:
        IOComponet* _ioc;
        int         _fd;
};

}
}
#endif
