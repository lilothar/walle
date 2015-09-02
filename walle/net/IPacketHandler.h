#ifndef WALLE_NET_IPACKETHANDLER_H_
#define WALLE_NET_IPACKETHANDLER_H_
namespace walle {
namespace net {

class IPacketHandler{
	public:
		enum HPCODE{
			KEEP_ALIVE,
			SHUT_DOWN

		};
	public:
		virtual ~IPacketHandler(){}
		virtual HPCODE handlerPacket(Packet*packet, void *args) = 0;

};


}

}
#endif
