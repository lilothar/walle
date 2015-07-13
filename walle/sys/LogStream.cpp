#include <walle/sys/wallesys.h>

namespace walle{
namespace sys {

LogStream& LogStream::operator<<(short v)
{
	if(_write) {
	 	*this<<static_cast<int>(v);
	}
	 return *this;
}
LogStream& LogStream::operator<<(unsigned short v)
{		
	if(_write) {
	 	*this<<static_cast<unsigned int>(v);
	}
	return *this;
}
LogStream& LogStream::operator<<(int v)
{
	if(_write) {
		appendInt(v);
	}
	return *this;
}
LogStream& LogStream::operator<<(unsigned int v)
{
	if(_write) {
		appendInt(v);
	}
	return *this;
}
LogStream& LogStream::operator<<(long v)
{
	if(_write) {
		appendInt(v);
	}
	return *this;

}
LogStream& LogStream::operator<<(unsigned long v)
{
	if(_write) {
		appendInt(v);
	}
	return *this;
}
LogStream& LogStream::operator<<(long long v)
{
	if(_write) {
		appendInt(v);
	}
	return *this;
}
LogStream& LogStream::operator<<(unsigned long long v) 
{
	if(_write) {
		appendInt(v);
	}
	return *this;
}
//  self& LogStream::operator<<(const Console::TextColor& color);
LogStream& LogStream::operator<<(const void* p)
{	if(_write) {
		uintptr_t v = reinterpret_cast<uintptr_t>(p);
		string ret = StringUtil::formateAddrHex(v);
		if (_buffer.avail() > ret.length()+2) {
			_buffer.append("0x",2);
			_buffer.append(ret.c_str(), ret.length());
		}
	}
	return *this;
}

LogStream& LogStream::operator<<(double v)
{
	if(_write) {

		string result = StringUtil::formateDouble(v);
		*this<<result;
	}
	return *this;
}
	


}
}
