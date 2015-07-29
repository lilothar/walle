#include <walle/net/http/HttpClientRequest.h>
#include <walle/sys/wallesys.h>
using namespace walle::sys;
namespace walle {
namespace http {
HttpClientRequest::HttpClientRequest():
	_version("HTTP/1.1"),
	_port(80)
{
	_headers.clear();
	_args.clear();
}
HttpClientRequest::~HttpClientRequest()
{

}

void	HttpClientRequest::setHost(const string &host)
{
	_host = host;
}
string	HttpClientRequest::getHost()
{
	return _host;
}

void   HttpClientRequest::setPort(const int &port)
{
	_port = port;
}
int  HttpClientRequest::getPort()
{
	return _port;
}

void   HttpClientRequest::addArg(const string& key, const string &value)
{
	_args[key] = value;
}
string HttpClientRequest::getArg(const string &key)
{
	DictItr itr = _args.find(key);
	if(itr != _args.end()) {
		return itr->second;
	}
	return string("");
}

void HttpClientRequest::clearArg() 
{
	_args.clear();
}
bool   HttpClientRequest::setMethod(ClientMethod mth)
{
	_method = mth;
	return true;
}

ClientMethod HttpClientRequest::getMethod()
{
	return _method;
}

void   HttpClientRequest::addHeader(const string &key, const string &value)
{
	assert(!key.empty());
	assert(!value.empty());
	_headers[key] = value;

}
void HttpClientRequest::clearHeader()
{
	_headers.clear();
}
void   HttpClientRequest::setContentType(const string &type)
{
	addHeader("Content-Type",type);	
}

void  HttpClientRequest:: setContentLeng(int64_t n)
{
	string strlen = walle::sys::StringUtil::formateInteger(n);
	addHeader("Content-Length",strlen);
}

string HttpClientRequest::getHeader(const string &key)
{
	DictItr itr = _headers.find(key);
	if(itr != _args.end()) {
		return itr->second;
	}
	return string("");
}

void   HttpClientRequest::setBody(const string &body)
{
	_body = body;
}
const string& HttpClientRequest::getBody() const
{
	return _body;
}

void HttpClientRequest::clearAll()
{
	_host.clear();
	_args.clear();
	_method = kCnomethod;
	_port = 0;
	_headers.clear();
	_body.clear();
}
bool HttpClientRequest::isMethodValid()
{
	if(_method ==  kCget ||
	   _method == kCput  ||
	   _method == kChead ||
	   _method == kCpost || 
	   _method == kCdelete 
	   ) {
		return true;
	}
	return false;
	
}

string	HttpClientRequest::getMethodString()
{
	if(_method == kCget ) {
		return "GET";
	} else if(_method == kCpost) {
		return "POST";
	} else if(_method == kCput) {
		return "PUT";
	} else if(_method == kCdelete) {
		return "DELETE";
	} else if(_method == kChead ) {
		return "HEAD";
	} else {
		return "";
	}
}

void   HttpClientRequest::setLocaltion(const string &l)
{
	_location = l;
}
string HttpClientRequest::getLocation()
{
	return _location;
}

bool  HttpClientRequest::toString(string &request)
{
	bool rc = true;
	if(!isMethodValid()) {
		rc = false;
		return rc;
	}

	request.append(getMethodString());
	request.append(1,' ');

	request.append(_location);

	if(!_args.empty()) {
		request.append(1, '?');
		DictItr begin = _args.begin();
		DictItr itr = _args.begin();
		while (itr != _args.end() )  {
			if(itr != begin) {
				request.append(1,'&');
			}
			request.append(itr->first);
			request.append(1,'=');
			request.append(itr->second);
			itr++;
		}//end while 	
	}//end if

	request.append(1, ' ');
	request.append(_version);
	request.append("\r\n");

	if(!_headers.empty()) {
		DictItr itr = _headers.begin();
		while (itr != _headers.end() )  {
			
			request.append(itr->first);
			request.append(": ");
			request.append(itr->second);
			request.append("\r\n");
			itr++;
		}//end while 	
	}//end if
	request.append("\r\n");
	request.append(_body);
	return true;
	
		
}
void   HttpClientRequest::setHttpVersion(const string&v )
{
	_version = v;
}
 string HttpClientRequest::getHttpVersion()
 {
		return _version;
 }

}
}
