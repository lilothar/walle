#ifndef WALLE_HTTPCLIENTREQUEST_H_
#define WALLE_HTTPCLIENTREQUEST_H_
#include <string>
#include <map>
#include <stdint.h>

using namespace std;
namespace walle {
namespace http {
 enum ClientMethod{
            kCnomethod,
            kCget,
            kCpost,
            kCput,
            kChead,
            kCdelete
        };
class HttpClientRequest{
    public:

        HttpClientRequest();
        ~HttpClientRequest();
        
        void    setHost(const string &host);
        string  getHost();
        
        void   setPort(const int &port);
        int    getPort();
        // like / or /index.html /test/test.html
        void   setLocaltion(const string &uri);
        string getLocation();
        void   addArg(const string& key, const string &value);
        string getArg(const string &key);
        void   clearArg();
        //like HTTP/1.1 HTTP/1.0
        void   setHttpVersion(const string&v );
        string getHttpVersion();
        bool   setMethod(ClientMethod mth);
        ClientMethod  getMethod();
        string        getMethodString();
        void   addHeader(const string &key, const string &value);
        string getHeader(const string &key);
        void   clearHeader();
        void   setContentLeng(int64_t n); 
        void   setContentType(const string &type);
        void   setBody(const string &body);
        const string& getBody() const;

        void clearAll();

        bool  toString(string &request);
    private:
        bool isMethodValid();
    private:
        typedef map<string, string >  Dict;
        typedef Dict::iterator DictItr;
        string  _version;
        string  _host;
        int     _port;
        string  _location;
        ClientMethod     _method;
        Dict    _args;
        Dict    _headers;
        string  _body;
        
        
        
};
}
}
#endif