#ifndef WALLE_SERVICE_H_
#define WALLE_SERVICE_H_
#include <walle/sys/wallesys.h>
#include <string>
#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <walle/net/Eventloop.h>
#include <walle/net/Channel.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <walle/sys/wallesys.h>

using namespace walle::sys;

namespace walle {
namespace net {
    
class EventLoop;

class Service{
    public:
       Service();
       virtual ~Service();
       void addSignal(int sig);
       int main(int argc, char*argv[]);

       bool  isDaemon();
        
       static Service* getInstace();
    protected:
        void start();
        void stop();

        virtual int run() = 0;

        virtual void version();
        virtual void runDaemon();

        
    protected:
        virtual void onReadSignal();
       
    private:
        bool             _isdaemon;
        bool             _active; 
        int              _signalfd;
        string           _workdir;
        Config           _conf;

        string           _confFile;
        string           _pidFile;

        boost::scoped_ptr<Channel> _sigChannel;
        EventLoop        _loop;
        
        sigset_t         _sigmask;
        parser           _args;
        static Service  *_instance;

        
        
        
        

};

}

}
#endif
