#ifndef WALLE_NET_PACKET_H_
#define WALLE_NET_PACKET_H_
#include <walle/net/Buffer.h>
#include <stdint.h>

namespace walle {
namespace net {

class PacketHeader{
	public:
		PacketHeader():_type(0),_datalen(0){}
	public:
		int32_t _type;
		int32_t _datalen;
};

class Packet{
	public:

		Packet(){}

		virtual ~Packet(){}

		void setType(int type)
		{
			_pHeader._type = type;
		}

		int getType() 
		{
			return _pHeader._type;
		}

		virtual void free()
		{
			delete this;
		}

		virtual isRegularPacket()
		{
			return true;
		}
		PacketHeader * getHeader()
		{
			return &_pHeader;
		}
		void setHeader(PacketHeader *header)
		{
			if(header) {
				memcpy(&_pHeader, header, sizeof(PacketHeader));
			}
		}
		
		virtual bool encode(Buffer *output) = 0;

		virtual bool decode(Buffer*intput,PacketHeader *header) = 0;
	private:
		PacketHeader _pHeader;
		
		

};
}
}
#endif
