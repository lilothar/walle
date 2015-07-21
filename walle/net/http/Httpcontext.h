#ifndef WALLE_HTTPCONTEXT_H_
#define WALLE_HTTPCONTEXT_H_
#include <walle/net/http/Httprequest.h>

namespace walle {
namespace http{

class HttpContext
{
 public:
  enum HttpRequestParseState
  {
    kExpectRequestLine,
    kExpectHeaders,
    kExpectBody,
    kGotAll,
  };

  HttpContext()
    : _state(kExpectRequestLine)
  {
  }

  // default copy-ctor, dtor and assignment are fine

  bool expectRequestLine() const
  { return _state == kExpectRequestLine; }

  bool expectHeaders() const
  { return _state == kExpectHeaders; }

  bool expectBody() const
  { return _state == kExpectBody; }

  bool gotAll() const
  { return _state == kGotAll; }

  void receiveRequestLine()
  { _state = kExpectHeaders; }

  void receiveHeaders()
  { _state = kExpectBody; }
  void receiveBody()
  {
	_state =kGotAll;
  }

  void reset()
  {
    _state = kExpectRequestLine;
    HttpRequest dummy;
    _request.swap(dummy);
  }

  const HttpRequest& request() const
  { return _request; }

  HttpRequest& request()
  { return _request; }

 private:
  HttpRequestParseState _state;
  HttpRequest _request;
};

}
}
#endif