#include <walle/net/wallenet.h>
#include <walle/net/http/HttpClient.h>
#include <boost/bind.hpp>

using namespace walle::net;
using namespace walle::http;

void mycb(HttpClientRequest*req, HttpClientResponse*res)
{
	LOG_ERROR<<res->getBody();
}
int main()
{
	EventLoopThread eth;
	EventLoop *loop = eth.startLoop();
	HttpClient httpclient(loop);
	HttpClientRequest &req = httpclient.getRequest();
	req.setHttpVersion("HTTP/1.1");
	req.setMethod(kCpost);
	req.setHost("127.0.0.1");
	req.setPort(8080);
	req.setLocaltion("/");
	req.setBody("hello");
	req.addHeader("Content-Length","4");
	httpclient.setResponseCallback(boost::bind(mycb,_1,_2));
	httpclient.start();
	while(1) {
		sleep(1);
	}

}
