#include <walle/sys/wallesys.h>
namespace walle {
namespace sys {

static const char* EXCEPTION_PREFIX = "EXCEPTION";

const char*Exception::what() const throw()
{
	_what = getErrorMsg();
	const char *ret = _what.c_str();
	return ret;
}
const std::string Exception::makeErrorLog()
{
	return string(EXCEPTION_PREFIX) + getErrorMsg();
}
SimpleException::SimpleException(const char *msg,
	const char* file ,
	const int line):_msg(msg),_file(file),_line(line)
{

}

const std::string SimpleException::getErrorMsg()
{
	return _msg;
}
const std::string SimpleException::makeErrorLog()
{
		char buff[1024];
		if(!_file.empty() && _line >= 0) {
			snprintf(buff, sizeof(buff)-1,"[%s] (%s:%d) %s",EXCEPTION_PREFIX,
				_file.c_str(),
				_line, _msg.c_str());
		} else {
			snprintf(buff, sizeof(buff)-1,"[%s] (%s:%d) %s",EXCEPTION_PREFIX,
				"N/A",
				0, _msg.c_str());
		}
		std::string result(buff);
		return result;
}

}
}
