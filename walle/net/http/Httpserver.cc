
#include <walle/net/http/Httpserver.h>


#include <walle/net/http/Httpcontext.h>
#include <walle/net/http/Httprequest.h>
#include <walle/net/http/Httpresponse.h>
#include <walle/sys/Logging.h>


using namespace walle::net;
using namespace walle::sys;

namespace walle {
namespace http {

namespace detail
{

// FIXME: move to HttpContext class
bool processRequestLine(const char* begin, const char* end, HttpContext* 
context)
{
  bool succeed = false;
  const char* start = begin;
  const char* space = std::find(start, end, ' ');
  HttpRequest& request = context->request();
  if (space != end && request.setMethod(start, space))
  {
    start = space+1;
    space = std::find(start, end, ' ');
    if (space != end)
    {
      const char* question = std::find(start, space, '?');
      if (question != space)
      {
        request.setPath(start, question);
        request.setQuery(question, space);
      }
      else
      {
        request.setPath(start, space);
      }
      start = space+1;
      succeed = end-start == 8 && std::equal(start, end-1, "HTTP/1.");
      if (succeed)
      {
        if (*(end-1) == '1')
        {
          request.setVersion(HttpRequest::kHttp11);
        }
        else if (*(end-1) == '0')
        {
          request.setVersion(HttpRequest::kHttp10);
        }
        else
        {
          succeed = false;
        }
      }
    }
  }
  return succeed;
}

bool parseRequest(Buffer* buf, HttpContext* context, Time receiveTime)
{
  bool ok = true;
  bool hasMore = true;
  while (hasMore) {
    if (context->expectRequestLine()) {
      const char* crlf = buf->findCRLF();
      if (crlf) {
        ok = processRequestLine(buf->peek(), crlf, context);
        if (ok) {
          context->request().setReceiveTime(receiveTime);
          buf->retrieveUntil(crlf + 2);
          context->receiveRequestLine();
        } else {
     
          hasMore = false;
        }
      } else {
        hasMore = false;
      }
    } else if (context->expectHeaders()) {
      const char* crlf = buf->findCRLF();
      if (crlf) {
        const char* colon = std::find(buf->peek(), crlf, ':');
        if (colon != crlf) {
          context->request().addHeader(buf->peek(), colon, crlf);
        }  else {
          // empty line, end of header
          context->receiveHeaders();
          hasMore = !context->gotAll();
        }
        buf->retrieveUntil(crlf + 2);
      } else {
        hasMore = false;
      }
    } else if (context->expectBody()) {
    	HttpRequest& req = context->request();
		size_t cachelen = req.getCachelen();
		
    	if(cachelen == 0) {
    		string lenstr = req.getHeader("Content-Length");
			if(lenstr.empty()) {
				context->receiveBody();
				hasMore = false;
				continue;
			}
			int64_t len = StringUtil::atoi(lenstr.c_str(),lenstr.length());
			if(len == 0 ) {
				context->receiveBody();
				hasMore = false;
				continue;
			}
			cachelen = (size_t)len;
			req.setCachelen(cachelen);
			
    	}
	   size_t bufflen = buf->readableBytes();
	   if(bufflen >= cachelen) {
	   		req.setBody(buf, cachelen);
			buf->retrieve(cachelen);
			context->receiveBody();
			hasMore = false;
			continue;
	   } else {
			hasMore = false;
	   }
	  
    }
    	
  }
  return ok;
}

void defaultHttpCallback(const HttpRequest&, HttpResponse* resp)
{
  resp->setStatusCode(HttpResponse::k404NotFound);
  resp->setStatusMessage("Not Found");
  resp->setCloseConnection(true);
}

}


HttpServer::HttpServer(EventLoop* loop,
                       AddrInet& listenAddr,
                       const string& name,
                       TcpServer::Option option)
  : _server(loop, listenAddr, name, option),
    _httpCallback(detail::defaultHttpCallback)
{
  _server.setConnectionCallback(
      std::bind(&HttpServer::onConnection, this, std::_1));
  _server.setMessageCallback(
      std::bind(&HttpServer::onMessage, this, std::_1, std::_2, std::_3));
}

HttpServer::~HttpServer()
{
}

void HttpServer::start()
{
  _server.start();
}

void HttpServer::onConnection(const TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    conn->setContext(HttpContext());
	 HttpContext* context = std::any_cast<HttpContext>(conn->getMutableContext());
	 string addr = conn->peerAddress().toString();
	 context->request().setClientIp(addr);
  }
}

void HttpServer::onMessage(const TcpConnectionPtr& conn,
                           Buffer* buf,
                           Time receiveTime)
{
  HttpContext* context = std::any_cast<HttpContext>(conn->getMutableContext());

  if (!detail::parseRequest(buf, context, receiveTime))
  {
  	LOG_ERROR<<"parse error: "<<buf;
    conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
    conn->shutdown();
  }

  if (context->gotAll())
  {
    onRequest(conn, context->request());
    context->reset();
  }
}

void HttpServer::onRequest(const TcpConnectionPtr& conn, const HttpRequest& 
req)
{
  const string& connection = req.getHeader("Connection");
  bool close = connection == "close" ||
    (req.getVersion() == HttpRequest::kHttp10 && connection != "Keep-Alive");
  HttpResponse response(close);
  _httpCallback(req, &response);
  Buffer buf;
  response.appendToBuffer(&buf);
  conn->send(&buf);

  if (response.closeConnection())
  {
    conn->shutdown();
  }
}
}
}
