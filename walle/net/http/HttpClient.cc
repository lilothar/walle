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
		delete _client;
		_client = NULL;
	}
 }
HttpClientRequest& HttpClient::getRequest()
{
	return _request;
}

HttpClientResponse& HttpClient::getResponse()
{
	return _response;
}

void HttpClient::start()
{
	AddrInet serverAddr(_request.getHost().c_str(), _request.getPort());
	
	_client = new TcpClient(_loop,serverAddr,"HttpClient");
	_client->setConnectionCallback(boost::bind(&HttpClient::onConnection,this,_1));
	_client->setMessageCallback(boost::bind(&HttpClient::onMessage,this,_1,_2,_3));
	_client->setWriteCompleteCallback(boost::bind(&HttpClient::onWriteComplete,this,_1));
	_client->connect();
}
void HttpClient::stop()
{
	if(_client) {
		_client->disconnect();
	}
}
	   
void HttpClient::setResponseCallback(HttpClientCallback cb)
{
	_onResponse = cb;
}

void HttpClient::setHttpCloseCallback(HttpClientCallback cb)
{
	_onClose = cb;
}
void HttpClient::sendqury()
{
		string requeststr;
		_request.toString(requeststr);
		_client->connection()->send(requeststr);
}

void HttpClient::onConnection(const TcpConnectionPtr& conn)
{
	if(!conn->connected()) {
		if(_onClose) {
			_onClose(this);
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
		if(_onResponse) {
			_onResponse(this);
		}
	}
}

}
}
