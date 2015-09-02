#ifndef WALLE_NET_TCPSERVERSM_H_
#define WALLE_NET_TCPSERVERSM_H_
namespace walle {
namespace net{
class TcpServerSM{
	public:
		TcpServerSM();	
		bool start();

		void stop();

	private:
		void onConnection(const TcpConnectionPtr& conn);
		void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Time time);

		void onWriteComplete(const TcpConnectionPtr& conn);

		void onHighWater(const TcpConnectionPtr& conn, size_t size);
		
	private:
		TcpServer       *_server;
		int              _threadnum;
		IPacketHandler  *_pHandler;
		IPacketStream   *_pStream; 
		

};
}
}
#endif
