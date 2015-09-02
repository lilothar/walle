#ifndef WALLE_NET_IPACKETSTREAM_H_
#define WALLE_NET_IPACKETSTREAM_H_

namespace walle{
namespace net {

class IPacketStream{
	public:
		IPacketStream():_hasHeader(true),_factory(NULL)
		{
		}

		IPacketStream(IPacketFactory * fact):_hasHeader(true),_factory(fact)
		{
		}

		virtual ~IPacketStream() {}
		
		virtual bool getPacketInfo(Buffer *buf, PacketHeader*header) = 0;

		virtual Packet* decode(Buffer *buf,PacketHeader *header) = 0;

		virtual bool encode(Packet* packet, Buffer *output) = 0;

		virtual bool hasHeader()
		{
			return _hasHeader;
		}
		
	private:
		bool            _hasHeader;
		IPacketFactory *_factory;
};
}
}
#endif
