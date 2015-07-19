#include <walle/net/Netcount.h>
#include <walle/sys/Logging.h>
using namespace walle::sys;
namespace walle{
namespace net {
	NetCount NetCount::_gNetCount;
	
	/*
	 * 构造函数
	 */
	NetCount::NetCount() {
		clear();
	}
	
	/*
	 * 析构函数
	 */
	NetCount::~NetCount() {
	}
	
	/*
	 * 把stat写到log中
	 */
	void NetCount::log() {
		LOG_INFO<<"dataReadCnt: "<<_dataReadCnt.get()<<"dataWriteCnt: "<< _dataWriteCnt.get();
	}
	
	/*
	 * 清空
	 */
	void NetCount::clear() {
		_dataReadCnt.getAndSet(0);
		_dataWriteCnt.getAndSet(0);
	}
}
}
