#include <walle/net/http/HttpClient.h>
#include <boost/bind.hpp>

using namespace walle::net;
using namespace walle::sys;

namespace walle {
namespace http{

 HttpClient::HttpClient(EventLoop *loop):_loop(loop)
 {}
 HttpClient::~HttpClient()
 {
	if(_client) {
		_client->disconnect();
		delete _client;
		_client = NULL;
	}
 }
       HttpClientRequest* getRequest();

void HttpClient::start()
{
	AddrInet serverAddr(_request.getHost().c_str(), _request.getPort());
	
	_client = new TcpClient(_loop,serverAddr,"HttpClient");
	_client->setConnectionCallback(boost::bind(&HttpClient::onConnection,this,_1));
	_client->setMessageCallback(boost::bind(&HttpClient::onMessage,this,_1,_2,_3));
	_client->setConnectionCallback(boost::bind(&HttpClient::onWriteComplete,this,_1));
	_client->connect();
}
void HttpClient::stop()
{
	if(_client) {
		_client->disconnect();
		delete _client;
		_client = NULL;	
	}
}
	   
void HttpClient::setResponseCallback(ResponseCb cb)
{
	_cb = cb;
}


void HttpClient::onConnection(const TcpConnectionPtr& conn)
{
	if(!conn->connected()) {
		_response.setValid(false);
		stop();
		if(_cb) {
			_cb(&_request, &_response);
		}
	} else {
		string requeststr;
		_request.toString(requeststr);
		conn->send(requeststr);
	
	}
}
void HttpClient::onWriteComplete(const TcpConnectionPtr& conn)
{
	LOG_DEBUG<<"send request complete wait for response";
}

void HttpClient::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Time time)
{
	const char* str = buf->peek();
	size_t readablesize = buf->readableBytes();
	size_t parsedsize = _response.parseResponse(str,readablesize);
	
	if(parsedsize > 0) {
		buf->retrieve(parsedsize);
	}
	if(_response.getAll()|| !_response.isvalid()) {
		if(_cb) {
			_cb(&_request,&_response);
		}
	}
}

}
}
