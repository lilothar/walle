#ifndef WALLE_NET_IPACKETFACTORY_H_
#define WALLE_NET_IPACKETFACTORY_H_

namespace walle {
namespace net {

class Packet;
class IPacketFactory{
	public:
		virtual ~IPacketFactory() {}
		virtual Packet* createPacket(int type) = 0;
};
}
}
#endif