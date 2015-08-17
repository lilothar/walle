#include <walle/net/wallenet.h>
#include <boost/bind.hpp>
#include <boost/bind.hpp>
using namespace walle::net;

class HoleServer{
	public:
		
		HoleServer(EventLoop* loop,  AddrInet& listenAddr, int threadnum)
			:_recived(0),
			_loop(loop),
			  _server(loop, listenAddr, "holeServer")
		  {
			_server.setConnectionCallback(
				boost::bind(&HoleServer::onConnection, this, _1));
			_server.setMessageCallback(
				boost::bind(&HoleServer::onMessage, this, _1, _2, _3));
			_server.setThreadNum(threadnum);
			_loop->runEvery(3000*1000,boost::bind(&HoleServer::log, this));
		  }
	void start()
  	{
    	_server.start();
  	}

private:
  void log() {
	LOG_INFO<<"recived bytes: "<<_recived;
  }
	
  void onConnection(const TcpConnectionPtr& conn)
  {
    LOG_TRACE << conn->peerAddress().toString() << " -> "
        << conn->localAddress().toString() << " is "
        << (conn->connected() ? "UP" : "DOWN");
    LOG_INFO << conn->getTcpInfoString();

   // conn->send("hello\n");
  }
	
  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Time time)
  {
  	_recived +=buf->readableBytes();
    buf->retrieveAll();
	//LOG_INFO<<"recived bytes: "<<_recived;
  }
	private:
	size_t     _recived;
	EventLoop* _loop;
  	TcpServer _server;
};


class app:public Service{
	public:
		app():_ser(0){}
		virtual ~app() {}
	private:
		virtual int run()
		{
			/*_serverBaseloop = _th.startLoop();
			int thnum = _conf.getInt("global","thread_num");
			if(thnum == 0) {
				thnum = 1;
			}
			string serveraddr = _conf.getString("global","server_listen_addr");
			AddrInet netaddr(serveraddr.c_str());
			_ser = new HoleServer(_serverBaseloop,netaddr,thnum);
			_ser->start();
			*/
			return 0;
		}
		virtual void dispose() 
		{
		/*
			delete _ser;
			_serverBaseloop->quit();
			_th.join();
			delete this;
			*/
		}
	private:
		HoleServer *_ser;
		EventLoopThread _th;
		EventLoop      *_serverBaseloop;
};


int main(int argc, char*argv[])
{
	Service *ap = new app();
	ap->main(argc,argv);
	return 0;
}
