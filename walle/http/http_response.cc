#include <cstdio>
#include <functional>
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <walle/http/http_utils.h>
#include <walle/http/http_response.h>
#include <walle/http/HttpResponse_builder.h>

using namespace std;

namespace walle {

namespace http{

HttpResponse::HttpResponse(const HttpResponse_builder& builder):
    _content(builder._content_hook),
    _responseCode(builder._response_code),
    _autodelete(builder._autodelete),
    _realm(builder._realm),
    _opaque(builder._opaque),
    _reloadNonce(builder._reload_nonce),
    _fp(-1),
    _filename(builder._content_hook),
    _headers(builder._headers),
    _footers(builder._footers),
    _cookies(builder._cookies),
    _topics(builder._topics),
    _keepaliveSecs(builder._keepalive_secs),
    _keepaliveMsg(builder._keepalive_msg),
    _sendTopic(builder._send_topic),
    _completed(false)
{
}

HttpResponse::~HttpResponse()
{
  
}

size_t HttpResponse::getHeaders(std::map<std::string, std::string, header_comparator>& result) const
{
    result = this->_headers;
    return result.size();
}

size_t HttpResponse::getFooters(std::map<std::string, std::string, header_comparator>& result) const
{
    result = this->_footers;
    return result.size();
}

size_t HttpResponse::getCookies(std::map<std::string, std::string, header_comparator>& result) const
{
    result = this->_cookies;
    return result.size();
}

}
}
