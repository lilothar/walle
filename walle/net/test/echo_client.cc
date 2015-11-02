#include <walle/sys/wallesys.h>
#include <walle/net/wallenet.h>
#include <boost/bind.hpp>

using namespace walle::net;
using namespace walle::sys;

int numThreads = 0;
class Echoclient;
boost::ptr_vector<Echoclient> clients;
size_t current = 0;

class Echoclient{
	public:
		Echoclient(EventLoop* loop,  AddrInet& listenAddr, const string& id): _loop(loop),
								_client(loop, listenAddr, "EchoClient"+id)
		{
			_client.setConnectionCallback(boost::bind(&Echoclient::onConnection, this, _1));
			_client.setMessageCallback(boost::bind(&Echoclient::onMessage, this, _1, _2, _3));
		}
		~Echoclient()
		{
		}
		void connect()
		{
			_client.connect();
		}
	private:
		void onConnection(const TcpConnectionPtr& conn)
		{
			LOG_TRACE << conn->localAddress().toString() << " -> "
				<< conn->peerAddress().toString() << " is "
				<< (conn->connected() ? "UP" : "DOWN");
			if (conn->connected())    {
				++current;      
				if (current < clients.size())      {
					clients[current].connect();      
				}      
				LOG_INFO << "*** connected " << current;    
			}
			 conn->send("world\n");

		}

		void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Time time)
		{
			string msg(buf->retrieveAllAsString());
			LOG_TRACE << conn->name() << " recv " << msg.size() << " bytes at " << time.toDateTime();
	    
			conn->send(msg);

		}
		EventLoop *_loop;
		TcpClient _client;

};


int main(int argc, char* argv[])
{  
	LOG_INFO << "pid = " << getpid() << ", tid = " << walle::LocalThread::tid();  
	if (argc > 1)  {
		EventLoop loop;
		AddrInet serverAddr(argv[1], 2000);
		int n = 1;
		if (argc > 2)    {
			n = atoi(argv[2]);
		}
		clients.reserve(n);
		for (int i = 0; i < n; ++i)    { 
			char buf[32];      snprintf(buf, sizeof buf, "%d", i+1);
			clients.push_back(new Echoclient(&loop, serverAddr, buf));
		} 
		clients[current].connect();
		loop.loop();
	}  else  {
			printf("Usage: %s host_ip [current#]\n", argv[0]);
	}

}

