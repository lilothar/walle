#ifndef WALLE_HTTPCLIENT_H_
#define WALLE_HTTPCLIENT_H_
#include <walle/net/wallenet.h>
#include <walle/sys/wallesys.h>
#include <walle/net/http/HttpClientRequest.h>
#include <walle/net/http/HttpClientResponse.h>
#include <boost/function.hpp>


using namespace walle::net;
using namespace walle::sys;

namespace walle {
namespace http {

 typedef boost::function<void(HttpClientRequest*,HttpClientResponse*)> ResponseCb;   
class HttpClient{
    public:
       HttpClient(EventLoop *loop);
       ~HttpClient();
       HttpClientRequest& getRequest();

       void start();
       void stop();
       void setResponseCallback(ResponseCb cb);
    private:
        void onConnection(const TcpConnectionPtr& conn);
        void onWriteComplete(const TcpConnectionPtr& conn);
        void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Time time);
   
        EventLoop         *_loop;
        TcpClient         *_client;
        HttpClientRequest  _request;
        HttpClientResponse _response;
        ResponseCb         _cb;


};
}
}
#endif
