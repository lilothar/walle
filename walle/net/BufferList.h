#ifndef WALLE_NET_LOCKEDLIST_H_
#define WALLE_NET_LOCKEDLIST_H_
#include <list>
#include <stddef.h>
#include <walle/sys/Mutex.h>
#include <walle/sys/ScopeLock.h>

using std::list;
namespace walle {
namespace net { 

class BufferList{
public:
	BufferList():_bufferSize(0){}
	~BufferList()
	{
		
	}

	void put(Buffer *pbuf)
	{
		walle::sys::ScopeLock lock(&_mutex);
		_buffers.push_back(pbuf);
		_bufferSize += pbuf->readableBytes();
	}

	Buffer * take()
	{
		
	}

	size_t bufferedSize();
private:
	size_t            _bufferSize;
	list<Buffer*>     _buffers;
	walle::sys::Mutex _mutex;
	
};
}
}