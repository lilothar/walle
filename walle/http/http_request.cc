#include <walle/http/http_utils.h>
#include <walle/http/http_request.h>
#include <algorithm>


using namespace std;

namespace walle  {
namespace http {

void HttpRequest::setMethod(const std::string& method)
{
    _method = method;
	std::transform(method.begin(),
		method.end(), method.begin(),
		(int(*)(int)) std::toupper);
}

size_t HttpRequest::getHeaders(std::map<std::string, std::string, http::header_comparator>& result) const
{
    result = this->headers;
    return result.size();
}

size_t HttpRequest::getFooters(std::map<std::string, std::string, http::header_comparator>& result) const
{
    result = this->footers;
    return result.size();
}

size_t HttpRequest::getCookies(std::map<std::string, std::string, http::header_comparator>& result) const
{
    result = this->cookies;
    return result.size();
}

size_t HttpRequest::getArgs(std::map<std::string, std::string, http::arg_comparator>& result) const
{
    result = this->args;
    return result.size();
}


}
}
