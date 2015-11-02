#include <walle/sys/wallesys.h>

int main()
{
	walle::sys::Logger::setLogLevel(walle::sys::Logger::TRACE);
	LOG_TRACE<<"I am trace";
	walle::sys::Logger::setLogLevel(walle::sys::Logger::DEBUG);
	LOG_TRACE<<"I am trace";
	LOG_DEBUG<<"show debug";
	walle::sys::Logger::setLogLevel(walle::sys::Logger::INFO);
	LOG_DEBUG<<"not show";
	LOG_INFO<<"show me";
	LOG_WARN<<"show warn";
	LOG_ERROR<<"show error";
	
}
