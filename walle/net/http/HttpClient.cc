#include <walle/net/http/HttpClient.h>

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
	_client->setConnectionCallback(std::bind(&HttpClient::onConnection,this, std::_1));
	_client->setMessageCallback(std::bind(&HttpClient::onMessage,this, std::_1, std::_2, std::_3));
	_client->setWriteCompleteCallback(std::bind(&HttpClient::onWriteComplete,this, std::_1));
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
	(void)conn;
	LOG_DEBUG<<"send request complete wait for response";
}

void HttpClient::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Time time)
{
	(void)conn;
	(void)time;
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
