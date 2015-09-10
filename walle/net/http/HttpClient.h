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


class HttpClient;

typedef boost::function<void(HttpClient*)> HttpClientCallback; 
  

class HttpClient{
    public:
       HttpClient(EventLoop *loop);
       ~HttpClient();
       HttpClientRequest& getRequest();
       HttpClientResponse& getResponse();
       void start();
       void stop();
       void sendqury();
       void setResponseCallback(HttpClientCallback cb);
       void setHttpCloseCallback(HttpClientCallback cb);
    private:
        void onConnection(const TcpConnectionPtr& conn);
        void onWriteComplete(const TcpConnectionPtr& conn);
        void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Time time);
   
        EventLoop              *_loop;
        TcpClient              *_client;
        HttpClientRequest       _request;
        HttpClientResponse      _response;
        HttpClientCallback      _onResponse;
        HttpClientCallback      _onClose;



};
}
}
#endif
