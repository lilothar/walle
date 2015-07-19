#ifndef WALLE_HTTPREQUEST_H_
#define WALLE_HTTPREQUEST_H_
#include <map>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <walle/sys/wallesys.h>
#include <cstring>
using namespace walle::sys;
namespace walle{
namespace http {
class HttpRequest 
{
 public:
  enum Method
  {
    kInvalid, kGet, kPost, kHead, kPut, kDelete
  };
  enum Version
  {
    kUnknown, kHttp10, kHttp11
  };

  HttpRequest()
    : _method(kInvalid),
      _version(kUnknown)
  {
  }

  void setVersion(Version v)
  {
    _version = v;
  }

  Version getVersion() const
  { return _version; }

  bool setMethod(const char* start, const char* end)
  {
    assert(_method == kInvalid);
    string m(start, end);
    if (m == "GET")
    {
      _method = kGet;
    }
    else if (m == "POST")
    {
      _method = kPost;
    }
    else if (m == "HEAD")
    {
      _method = kHead;
    }
    else if (m == "PUT")
    {
      _method = kPut;
    }
    else if (m == "DELETE")
    {
      _method = kDelete;
    }
    else
    {
      _method = kInvalid;
    }
    return _method != kInvalid;
  }

  Method method() const
  { return _method; }

  const char* methodString() const
  {
    const char* result = "UNKNOWN";
    switch(_method)
    {
      case kGet:
        result = "GET";
        break;
      case kPost:
        result = "POST";
        break;
      case kHead:
        result = "HEAD";
        break;
      case kPut:
        result = "PUT";
        break;
      case kDelete:
        result = "DELETE";
        break;
      default:
        break;
    }
    return result;
  }

  void setPath(const char* start, const char* end)
  {
    _path.assign(start, end);
  }

  const string& path() const
  { return _path; }

  void setQuery(const char* start, const char* end)
  {
    _query.assign(start, end);
    parseQuery();
  }
  void parseQuery()
  {
    _args.clear();
    const char *pquery = _query.c_str();
    const char* start;
    const char * end;
    int          leave_size;
    while (*pquery == ' ' || *pquery =='?') {
        pquery++;
    }
    leave_size = strlen(pquery);
    while(leave_size) {
         start = pquery;
         end = strstr(start, "=");
         while (*end == ' ') end--;
         string key(start,end - start);
         start = end;
         while (*start == ' ' || *start == '=') {
            start++;
         }
         end = strstr(start, "&");
         if (!end) {
            end = start + strlen(start);
         }
         pquery = end;
         while (*end == ' ') end--;
         string value(start,end - start);
         _args[key] = value;
         while (*pquery == ' ' || *pquery =='&') {
            pquery++;
         }
         leave_size = strlen(pquery);
   }
  }
  const string& query() const
  { return _query; }

  void setReceiveTime(Time t)
  { _receiveTime = t; }

  Time receiveTime() const
  { return _receiveTime; }

  void addHeader(const char* start, const char* colon, const char* end)
  {
    string field(start, colon);
    ++colon;
    while (colon < end && isspace(*colon))
    {
      ++colon;
    }
    string value(colon, end);
    while (!value.empty() && isspace(value[value.size()-1]))
    {
      value.resize(value.size()-1);
    }
    _headers[field] = value;
  }

  string getHeader(const string& field) const
  {
    string result;
    std::map<string, string>::const_iterator it = _headers.find(field);
    if (it != _headers.end())
    {
      result = it->second;
    }
    return result;
  }
  const std::map<string, string>& args() const
  { return _args; }

  const std::map<string, string>& headers() const
  { return _headers; }

  void swap(HttpRequest& that)
  {
    std::swap(_method, that._method);
    _path.swap(that._path);
    _query.swap(that._query);
    Time tmp = _receiveTime;
    _receiveTime = that._receiveTime;
    that._receiveTime = tmp;
    _headers.swap(that._headers);
  }

 private:
  Method                   _method;
  Version                  _version;
  string                   _path;
  string                   _query;
  Time                     _receiveTime;
  std::map<string, string> _headers;
  std::map<string, string> _args;
};

}
}
#endif
