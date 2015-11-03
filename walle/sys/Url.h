#ifndef WALLE_SYS_URL_H_
#define WALLE_SYS_URL_H_
#include <string>
#include <stdint.h>

using std::string;

namespace walle {
namespace util {
class Url
{
public:
    // The URL parts.
    enum URL_PART
    {
        URL_PROTOCOL  = 0x0001,
        URL_HOST      = 0x0002,
        URL_PORT      = 0x0004,
        URL_PATH      = 0x0008,
        URL_FILENAME  = 0x0010,
        URL_BOOKMARK  = 0x0020,
        URL_USERNAME  = 0x0040,
        URL_PASSWORD  = 0x0080,
        URL_PARAMS    = 0x0100,
        URL_ALL       = 0xFFFF,
    };

public:
    Url(const string& url = "");
    Url(const Url& src);
    virtual ~Url() {}

    void clear();
    Url& operator = (const Url& rhs);

    string getUrl() const;
    string getUrl(uint32_t parts);
    void setUrl(const string& value);

    const string& getProtocol() const { return _protocol; }
    const string& getHost() const { return _host; }
    const string& getPort() const { return _port; }
    const string& getPath() const { return _path; }
    const string& getFileName() const { return _fileName; }
    const string& getBookmark() const { return _bookmark; }
    const string& getUserName() const { return _userName; }
    const string& getPassword() const { return _password; }
    const string& getParams() const { return _params; }

    void setProtocol(const string& value) { _protocol = value; }
    void setHost(const string& value) { _host = value; }
    void setPort(const string& value) { _port = value; }
    void setPath(const string& value) { _path = value; }
    void setFileName(const string& value) { _fileName = value; }
    void setBookmark(const string& value) { _bookmark = value; }
    void setUserName(const string& value) { _userName = value; }
    void setPassword(const string& value) { _password = value; }
    void setParams(const string& value) { _params = value; }

private:
    string _protocol;
    string _host;
    string _port;
    string _path;
    string _fileName;
    string _bookmark;
    string _userName;
    string _password;
    string _params;
};    
}
}
#endif
