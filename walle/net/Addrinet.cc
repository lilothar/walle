#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <walle/net/Addrinet.h>

#if defined(__clang__) || __GNUC_MINOR__ >= 6
#pragma GCC diagnostic push
#endif
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wold-style-cast"

#define DL_MAX_DEFAULT_PORT 65535
namespace walle{
namespace net {

AddrInet::AddrInet()
{
	reset();
}
AddrInet::~AddrInet (void)
{

}
AddrInet::AddrInet(const sockaddr_in *addr, int len)
{
	set(addr,len);
}
AddrInet::AddrInet(const AddrInet& rhs)
{
	set(rhs);
}
AddrInet::AddrInet(const char* addr)
{
	set (addr);
}
AddrInet::AddrInet(const char* spip, int ipport)
{
	set(spip, ipport);
}

void AddrInet::reset(void)
{
	::memset (&this->_inet_addr, 0, sizeof (this->_inet_addr));
}

bool AddrInet::upDateLocal(int fd)
{
	if(fd == -1) {
		return false;
	}
	socklen_t len = sizeof(_inet_addr);
	return (0==::getsockname(fd, (struct sockaddr*)(&_inet_addr), &len));
}
bool AddrInet::set(const AddrInet & rhs)
{

	::memcpy (&this->_inet_addr,
	                      &rhs._inet_addr,
	                      sizeof(_inet_addr));
	return true;
}
bool AddrInet::set (const char* spip, int ipport)
{
	_inet_addr.sin_family = AF_INET;
	_inet_addr.sin_port   = htons(static_cast<uint16_t>(ipport));
	if(!spip||spip[0] == '\0')
	{
		_inet_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		char c;

		const char *p = spip;

		bool isIPAddr = true;

		//is ip format?
		while ((c = (*p++)) != '\0') {
		if ((c != '.') && (!((c >= '0') && (c <= '9')))) {
			isIPAddr = false;
			break;
		}
		}

		if (isIPAddr)
		{
			_inet_addr.sin_addr.s_addr = ::inet_addr(spip);
		}
		else
		{
			return false;
		}
	}
	return true;

}
bool AddrInet::set (const char*addr)
{
	const char *ipport = ::strrchr(addr,':');
	char spip[32] = {};
	int p;

	if(!ipport)
	{
		p = atoi(addr);
		if(p <0 || p >DL_MAX_DEFAULT_PORT)
		{
			return -1;
		}
		return set(spip, p);
	}
	else
	{
		memcpy(spip,addr,ipport-addr);
		p = atoi(ipport+1);
		return set(spip, p);
	}
}


bool AddrInet::set (const struct sockaddr_in * sa, int len)
{
	  int maxlen = static_cast<int> (sizeof (this->_inet_addr));
	  if(len > maxlen)
		len = maxlen;
	  ::memcpy (&this->_inet_addr, sa, len);
	  return true;
}


struct sockaddr_in*AddrInet::getAddr (void)
{
	return &_inet_addr;
}

std::string AddrInet::toString(bool flag) const
{
#pragma pack(1)
    union CIpUnion
    {
        uint32_t value;
        struct
        {
            unsigned char ch1;  // valueµÄ×îµÍ×Ö½Ú
            unsigned char ch2;
            unsigned char ch3;
            unsigned char ch4;
        } Bytes;
    } IpUnion;
#pragma pack()
    char str[64];
    uint32_t ipport = ntohs(_inet_addr.sin_port);
    IpUnion.value = ntohl(_inet_addr.sin_addr.s_addr);
    if (flag) {
    	sprintf(str, "%u.%u.%u.%u:%u", IpUnion.Bytes.ch4, IpUnion.Bytes.ch3,
    			IpUnion.Bytes.ch2, IpUnion.Bytes.ch1,ipport);
    } else {
    	sprintf(str, "%u.%u.%u.%u", IpUnion.Bytes.ch4, IpUnion.Bytes.ch3,
    	    			IpUnion.Bytes.ch2, IpUnion.Bytes.ch1);
    }
    return std::string(str);
}
uint32_t  AddrInet::port()
{
	uint32_t ipport = ntohs(_inet_addr.sin_port);
	return ipport;
}

uint64_t AddrInet::ipAndPort()
{
	uint64_t h;
	h = ntohs(_inet_addr.sin_port);
	uint32_t spip   = ntohl(_inet_addr.sin_addr.s_addr);
	h <<= 32;
	h |= spip;
	return h;
}

uint32_t AddrInet::ip()
{
	uint32_t spip   = ntohl(_inet_addr.sin_addr.s_addr);
	return spip;
}

bool AddrInet::operator == (const AddrInet &rhs) const
{
	  return (::memcmp (&this->_inet_addr,
	                          &rhs._inet_addr,
	                         sizeof(_inet_addr)) == 0);
}

bool AddrInet::operator != (const AddrInet &rhs) const
{
	return !(*this==rhs);
}

uint64_t  AddrInet::hash (void) const
{
	uint64_t h;
	h = ntohs(_inet_addr.sin_port);
	h <<= 32;
	h |= ntohl(_inet_addr.sin_addr.s_addr);
	return h;

}
}
}
