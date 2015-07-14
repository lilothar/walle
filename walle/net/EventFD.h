#ifndef WALLE_EVENTFD_H_
#define WALLE_EVENTFD_H_
namespace walle {
namespace net {
class IOComponent;

class EventFD{
    public:
        enum InvalidHandle{
            INVALIDHANDLE = -1
        };
        EventFD(){}
        virtual ~EventFD() {}
        virtual size_t read(char* buff, size_t len) = 0;
        virtual size_t write(char* buff, size_t len) = 0;
        virtual void   close();
        IOComponent *getIoc() { return _ioc; }
        void         setIoc(IOComponent *i) { _ioc = i; }
        bool         valid() { return _fd > 0; }
        int          getHandle() { return _fd; }
    protected:
        IOComponent *_ioc;
        int          _fd;
    
};

}
}
#endif
