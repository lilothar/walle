#ifndef WALLE_DB_STATUS_H_
#define WALLE_DB_STATUS_H_
#include <stdint.h>
#include <string>

using std::string;

namespace walledb {

class Status{
public:
	Status(int32_t errCode = 0, const string & errMsg = "")
		:_errcode(errCode),
		 _msg(errMsg)
	{
		
	}
	~Status() {}
	bool OK() { return _errcode == 0;}
	string errMsg() { return _msg; }
	int32_t errCode() { return _errcode; }
private:
	
	Status();
	int32_t _errcode;
	string  _msg;
};
}

#endif
