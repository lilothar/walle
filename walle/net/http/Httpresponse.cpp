#include <walle/net/http/Httpresponse.h>
#include <walle/net/Buffer.h>

#include <stdio.h>

using namespace walle::http;

void HttpResponse::appendToBuffer(Buffer* output) const
{
  char buf[32];
  snprintf(buf, sizeof buf, "HTTP/1.1 %d ", _statusCode);
  output->append(buf);
  output->append(_statusMessage);
  output->append("\r\n");

  if (_closeConnection)
  {
    output->append("Connection: close\r\n");
  }
  else
  {
    snprintf(buf, sizeof buf, "Content-Length: %zd\r\n", _body.size());
    output->append(buf);
    output->append("Connection: Keep-Alive\r\n");
  }

  for (std::map<string, string>::const_iterator it = _headers.begin();
       it != _headers.end();
       ++it)
  {
    output->append(it->first);
    output->append(": ");
    output->append(it->second);
    output->append("\r\n");
  }

  output->append("\r\n");
  output->append(_body);
}

