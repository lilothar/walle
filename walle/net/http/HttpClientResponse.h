#ifndef WALLE_HTTPCLIENT_RESPONSE_H_
#define WALLE_HTTPCLIENT_RESPONSE_H_
#include <string>
#include <map>
#include <vector>
#include <stdint.h>
#include <stddef.h>
using namespace std;

namespace walle {
namespace http {

enum HttpResponseParseStat{
            kCExceptStatLine,
            kCExceptHeader,
            kCExceptBody,
            kCGotAll
};


class HttpClientResponse {
    public:
    typedef map<string, string> Dict;
    typedef Dict::iterator      DictItr;
    public:
      HttpClientResponse();
      ~HttpClientResponse();
      void receiveStatLine();
      void receiveHeader();
      HttpResponseParseStat getStat();
      void receiveBody();

      bool getAll();
      bool isvalid();
      void setValid(bool valid);
      size_t parseResponse(const char* buff, size_t len);
      //list must be empty
      map<string, string>&   getHeaders();
      int       getHttpCode();
      string    getHttpStatLine();
      string    getBody();
    private:
      size_t    parseStatLine(const char* buff, size_t len);
      size_t    parseHeaders(const char* buff, size_t len);
      size_t    parseBody(const char* buff, size_t len);
      
    private:

        bool    _error;
        int     _httpcode;
        string  _version;
        string  _httpStatline;
        Dict    _headers;
        string  _body;
        size_t  _cachelen;
        HttpResponseParseStat _parseStat;
};

}

}
#endif