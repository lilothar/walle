#include <stdlib.h>
#include <walle/http/http_resource.h>
#include <walle/http/http_request.h>
#include <walle/http/http_response.h>
#include <walle/http/http_response_builder.hpp>

using namespace std;

namespace walle {
namespace http {
//RESOURCE
void HttpResource::resource_init()
{
    _allowedMethods[kHttpGet] = true;
    _allowedMethods[kHttpPost] = true;
    _allowedMethods[kHttpPut] = true;
    _allowedMethods[kHttpHeader] = true;
    _allowedMethods[kHttpDelete] = true;
    _allowedMethods[kHttpTrace] = true;
    _allowedMethods[kHttpConnect] = true;
    _allowedMethods[kHttpOption] = true;
}

namespace details
{

void empty_render(const HttpRequest& r, HttpResponse** res)
{
	(void)r;
    *res = new HttpResponse(http_response_builder("", 200).string_response());
}

};

}
}
