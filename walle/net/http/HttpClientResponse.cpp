#include <walle/net/http/HttpClientResponse.h>
#include <walle/sys/wallesys.h>
#include <algorithm>

namespace walle {

namespace http {

  HttpClientResponse::HttpClientResponse():
   					_error(false),
   					_httpcode(0),
   					_cachelen(0),
  					_parseStat(kCExceptStatLine)
  {
	_body.clear();
  }
  HttpClientResponse::~HttpClientResponse()
  {

  }
  void HttpClientResponse::receiveStatLine()
  {
	_parseStat = kCExceptHeader;
  }
  void HttpClientResponse::receiveHeader()
  {
	_parseStat = kCExceptBody;
  }
  HttpResponseParseStat HttpClientResponse::getStat()
  {
	return _parseStat;
  }
  void HttpClientResponse::receiveBody()
  {
	_parseStat = kCGotAll;
  }
  bool HttpClientResponse::isvalid()
  {
	return !_error;
  }
  void HttpClientResponse::setValid(bool valid)
  {
		_error = !valid;
  }

  bool HttpClientResponse::getAll()
  {
	return _parseStat == kCGotAll;
  }
  size_t HttpClientResponse::parseResponse(const char* buff, size_t len)
  {
  		size_t parseSize = 0;
		size_t segsize; 
		if( _parseStat == kCExceptStatLine) {
			segsize = parseStatLine(buff, len);
			parseSize +=  segsize;

		}		

		if(_parseStat == kCExceptHeader ) {
			segsize = parseHeaders(buff + parseSize , len - parseSize);
			parseSize +=  segsize;
		}

		if(_parseStat == kCExceptBody) {
			segsize = parseBody(buff + parseSize,len - parseSize);
			parseSize +=  segsize;
		}
		return parseSize;
  }
  map<string, string>&  HttpClientResponse::getHeaders()
  {
		return _headers;
  }
  
 int HttpClientResponse::getHttpCode()
  {
		return _httpcode;
  }
  string	HttpClientResponse::getHttpStatLine()
  {
		return _httpStatline;
  }
  string	HttpClientResponse::getBody()
  {
		return _body;
  }
  size_t	HttpClientResponse::parseStatLine(const char* buff, size_t len)
  {
		assert(buff);
		size_t parsed = 0;
		const char *c = "\r\n";
		const char * end = buff+len;
		const char *crlf = std::search(buff, end, c, c+2);
		if(crlf == end ) {
			return 0;
		}
		//version
		parsed = crlf - buff;
		const char *space = std::find(buff, crlf, ' ');
		
		_version =string(buff,space);
		
		while(*space == ' ') space++;
		const char *statmsg = std::find(space, crlf, ' ');
		_httpcode = (int)walle::sys::StringUtil::atoi(space,(size_t)(statmsg - space));
		
		while(*statmsg == ' ') statmsg++;
		
		_httpStatline = string(statmsg, crlf);
		receiveStatLine();
		return parsed+2;
		
  }
  size_t	HttpClientResponse::parseHeaders(const char* buff, size_t len)
  {
		const char *end = buff+len;
		const char *c = "\r\n";
		const char *crlf = std::search(buff, end, c, c+2);
		const char *start = buff;
		size_t parsed = 0;
		while (crlf != end) {
			const char* colon = std::find(start, crlf, ':');
			if(colon == crlf) {
				receiveHeader();
				parsed += 2;
				return parsed;
			}
			string key(start,colon);
			colon++;
			while(*colon == ' ') colon++;
			string value(colon,crlf);
			while(value[value.size()-1] == ' ') {
				size_t valuesize = value.size();
				value.resize(valuesize -1);
			}
			_headers[key] = value;
			parsed += crlf - start + 2;
			start = crlf + 2;
			
			crlf = std::search(start, end, c, c+2);
			
		}
		return parsed;
  }
size_t	HttpClientResponse::parseBody(const char* buff, size_t len)
{
	string contentLen("Content-Length");
	DictItr itr= _headers.find(contentLen);
	size_t parsed;
	if(itr == _headers.end()) {
		//no content
		receiveBody();
		return 0;
	}
	if(_cachelen == 0) {
		_cachelen = walle::sys::StringUtil::atoi(itr->second.c_str(),itr->second.length());
		_body.reserve(_cachelen);
	}

	if(len <= _cachelen - _body.size()) {
		parsed = len;
	} else {
		parsed = _cachelen - _body.size();
	}

	_body.append(buff,parsed);
	if(_body.size() == _cachelen) {
		receiveBody();
	}
	return parsed;
}
}
}
