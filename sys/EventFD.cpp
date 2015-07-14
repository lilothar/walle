#include <walle/sys/EventFD.h>
#include <unistd.h>
namespace walle {

namespace sys {

void EventFD::close()
{
	if(_fd > 0) {
		::close(_fd);
		_fd = -1;
	}
}
}
}
