#ifndef WALLE_HTTP_HTTP_REQUEST_PARSER_H_
#define WALLE_HTTP_HTTP_REQUEST_PARSER_H_

namespace walle {

namespace http {

class HttpRequestParser {
public:
	  enum HttpRequestParseState
  {
    kExpectRequestLine,
    kExpectHeaders,
    kExpectBody,
    kGotAll,
  };
public:
	HttpRequestParser();
	~HttpRequestParser();
	void reset();
	
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
	HttpRequest _request;
	HttpRequestParseState _state;
};

}
}
#endif