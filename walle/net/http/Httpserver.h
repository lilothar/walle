#ifndef WALLE_HTTPSERVER_H_
#define WALLE_HTTPSERVER_H_

#include <walle/net/Tcpserver.h>
#include <walle/net/Addrinet.h>
#include <walle/sys/Time.h>
#include <walle/net/Buffer.h>

using namespace walle::sys;
using namespace walle::net;
namespace walle {
namespace http {

class HttpRequest;
class HttpResponse;

class HttpServer : boost::noncopyable
{
 public:
  typedef boost::function<void (const HttpRequest&,
                                HttpResponse*)> HttpCallback;

  HttpServer(EventLoop* loop,
             AddrInet& listenAddr,
             const string& name,
             TcpServer::Option option = TcpServer::kNoReusePort);

  ~HttpServer();  // force out-line dtor, for scoped_ptr members.

  EventLoop* getLoop() const { return _server.getLoop(); }

  /// Not thread safe, callback be registered before calling start().
  void setHttpCallback(const HttpCallback& cb)
  {
    _httpCallback = cb;
  }

  void setThreadNum(int numThreads)
  {
    _server.setThreadNum(numThreads);
  }

  void start();

 private:
  void onConnection(const TcpConnectionPtr& conn);
  void onMessage(const TcpConnectionPtr& conn,
                 Buffer* buf, Time receiveTime);
  void onRequest(const TcpConnectionPtr&, const HttpRequest&);

  TcpServer _server;
  HttpCallback _httpCallback;
};

}
}


#endif
