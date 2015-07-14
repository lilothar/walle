#include <walle/net/EventFD.h>
#include <unistd.h>

namespace walle {
namespace net{
void EventFD::close()
{
	if(_fd != InvalidHandle) {
		::close(_fd);
		_fd = InvalidHandle;
	}
}

}

}
