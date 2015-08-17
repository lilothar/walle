#ifndef WALLE_HTTPREPONSE_H_
#define WALLE_HTTPREPONSE_H_

#include <string>
#include <map>
#include <walle/net/Buffer.h>
#include <walle/sys/StringUtil.h>

using namespace walle::net;
using namespace std;
namespace walle {
namespace http {
	
class HttpResponse 
{
 public:
  enum HttpStatusCode
  {
    kUnknown,
    k200Ok = 200,
    k301MovedPermanently = 301,
    k400BadRequest = 400,
    k404NotFound = 404,
  };

  explicit HttpResponse(bool close)
    : _statusCode(kUnknown),
      _closeConnection(close)
  {
  }

  void setStatusCode(HttpStatusCode code)
  { _statusCode = code; }

  void setStatusMessage(const string& message)
  { _statusMessage = message; }

  void setCloseConnection(bool on)
  { _closeConnection = on; }

  bool closeConnection() const
  { return _closeConnection; }

  void setContentType(const string& contentType)
  { addHeader("Content-Type", contentType); }

  void setContentLeng(int64_t n) 
  {
    string len = walle::sys::StringUtil::formateInteger(n);
    addHeader("Content-Length",len);
    
  }

  // FIXME: replace string with StringPiece
  void addHeader(const string& key, const string& value)
  { _headers[key] = value; }

  void setBody(const string& body)
  {
    _body = body;
    setContentLeng(_body.size());
  }

  void appendToBuffer(Buffer* output) const;

 private:
  std::map<string, string> _headers;
  HttpStatusCode _statusCode;
  string _statusMessage;
  bool _closeConnection;
  string _body;
};

}
}
#endif
