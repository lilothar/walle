#include <walle/net/wallenet.h>
#include <walle/net/http/HttpClient.h>
#include <walle/algo/functional.h>

using namespace walle::net;
using namespace walle::http;
bool run = true;
void mycb(HttpClient *hc)
{
	HttpClientResponse  & res = hc->getResponse();
	LOG_ERROR<<res.getBody();
	hc->stop();
}

void myclose(HttpClient *hc) 
{
	LOG_ERROR<<"close cb";
	delete hc;
	run = false;
}
int main()
{
	EventLoopThread eth;
	EventLoop *loop = eth.startLoop();
	HttpClient *httpclient = new HttpClient(loop);
	HttpClientRequest &req = httpclient->getRequest();
	req.setHttpVersion("HTTP/1.1");
	req.setMethod(kCpost);
	req.setHost("127.0.0.1");
	req.setPort(8080);
	req.setLocaltion("/");
	req.setBody("hello");
	req.addHeader("Content-Length","4");
	req.addArg("name","walle");
	httpclient->setResponseCallback(std::bind(mycb,std::_1));
	httpclient->setHttpCloseCallback(std::bind(myclose,std::_1));
	httpclient->start();
	while(run) {
		sleep(1);
	}
	loop->quit();

}
