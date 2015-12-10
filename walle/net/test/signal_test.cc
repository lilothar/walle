#include <walle/net/wallenet.h>
#include <walle/sys/wallesys.h>
#include <walle/algo/functional.h>

using namespace walle::net;
using namespace walle::sys;
int sfd;

int createSignalfd(int fd, sigset_t *mask)
{

	if(sigprocmask(SIG_BLOCK, mask, NULL) == -1) {
		LOG_ERROR<<"sigprocmask error errno:"<<errno;
		return -1;
	}

	
	int refd = signalfd(fd, mask ,SFD_NONBLOCK | SFD_CLOEXEC);
	
	return refd;
	
}
void handread(Time t)
{
	(void)t;
	struct signalfd_siginfo fdsi;
	memset(&fdsi,0,sizeof(fdsi));
	ssize_t s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
	if (s != sizeof(struct signalfd_siginfo)) {
		LOG_ERROR<<"read signal error size: "<<s
			<<"should be :"<<sizeof(struct signalfd_siginfo);
		return ;
	}
	if (fdsi.ssi_signo == SIGINT) {
		LOG_DEBUG<<"SIGINT signal to stop";
	}
	
	if (fdsi.ssi_signo == SIGKILL) {
		LOG_DEBUG<<"kill signal to stop";
	}
	LOG_WARN<<"recv signal: "<<fdsi.ssi_signo;


}
int main()
{
	sigset_t   mymask;
	sigemptyset(&mymask);
	sigaddset(&mymask, SIGINT);
	sigaddset(&mymask, SIGQUIT);
	sigaddset(&mymask, 9);
	sigaddset(&mymask, 3);
	sfd = createSignalfd(-1,&mymask);
	EventLoop loop;
	Channel   c(&loop,sfd);
	c.setReadCallback(std::bind(handread,std::_1));
	c.enableReading();
	loop.loop();
}

