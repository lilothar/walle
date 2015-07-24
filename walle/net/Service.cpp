#include <walle/net/Service.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


using namespace walle::sys;
using namespace std;
namespace walle {

namespace net {

namespace detail{

int createSignalfd(int fd, sigset_t *mask)
{
	sigprocmask(SIG_BLOCK, mask, NULL);
	if(sigprocmask(SIG_BLOCK, mask, NULL) == -1) {
		return -1;
	}
	
	signalfd(fd, mask ,SFD_NONBLOCK | SFD_CLOEXEC);	
	return fd;
	
}
}

Service::Service()
{
	sigemptyset(&_sigmask);
	sigaddset(&_sigmask, SIGINT);
	sigaddset(&_sigmask, SIGQUIT);
	
}

Service:: ~Service()
{
	if(_signalfd) {
		_sigChannel->disableAll();
  		_sigChannel->remove();
  		::close(_signalfd);
		_signalfd = -1;
	}
}

void Service::addSignal(int sig)
{
	sigaddset(&_sigmask, sig);
}

void Service::stop()
{
	_active = false;
	_loop.quit();
}
int Service::main(int argc, char* argv[])
{
	
	_isdaemon = false;
	
	_args.add("version", 'v', "show version");
	_args.add<string>("config", 'c', "config file path", false,"./service.conf");
	_args.add<string>("command", 'k', "user cmd", false);
	_args.parse_check(argc,argv);

	if(_args.exist("version")) {
		version();
		exit(0);
	}

	 _confFile =  _args.get<string>("config");
	 if(!_conf.load(_confFile)) {
		cout<<"Config file load error to check config file"<<endl;
		exit(1);
	 }
	string cmd  =  _args.get<string>("command");
	if(cmd == "start") {
		if(_confFile.empty()) {
			cout<<"must have config file to start"<<endl;
			exit(0);
		}
		start();	
		
	} else if(cmd == "stop") {
		_pidFile = _conf.getString("global","pidfile");
		if(_pidFile.empty()) {
			cout<<"must have pidfile to stop"<<endl;
		}
		string pidcontent;
		if(!Filesystem::fileExist(_pidFile)) {
			cout<<"proggram not running"<<endl;
			exit(0);
		}
		ReadSmallFile r(_pidFile);
		bool ret = r.read(&pidcontent,64);
		if(!ret) {
			 cout<<"[ERROR]: Not found pidfile["<<_pidFile<<"]..."<<endl;
		}
		
		int64_t opid = StringUtil::atoi(pidcontent);
		if(opid > 0) {
			if(kill(opid,SIGINT) != 0) {
				 cout<<"[ERROR]: kill pid["<<opid<<"] failed..."<<endl;
			}
		} else {
			 cout<<"[ERROR]: Not found pid["<<opid<<"]...."<<endl;
		}
		Filesystem::deleteFile(_pidFile);
		exit(0);	
		
	} 
	return 0;
	
}
void Service::runDaemon()
{
	int pid;
	int i;
	if((pid=fork()) > 0 ) {
		exit(0);
	} else if(pid < 0) {
		exit(1);
	}
	
	setsid();
	if((pid=fork()) > 0) {
		exit(0);
	} else if(pid< 0) {		
		exit(1);
	}

	for(i=0;i< NOFILE;++i) {
		close(i);
	}

	_workdir = _conf.getString("global","service_work_dir");
	if(!_workdir.empty()) {
		chdir(_workdir.c_str());
	}
	umask(0);
	return;	
}

void Service::start() 
{

	string tmp = _conf.getString("global", "daemon_mode");
	if(tmp == "true" || tmp == "TRUE") {
		_isdaemon = true;
	}

	_pidFile =  _conf.getString("global", "pidfile");
	
	if(_isdaemon && _pidFile.empty()) {
		
		cout<<"no pidfile configed"<<endl;
		exit(0);
	}

	runDaemon();
	
	_signalfd = detail::createSignalfd(_signalfd, &_sigmask);
	if(_signalfd < 0) {
		cerr<<"create signal fd error"<<endl;
	}
	_sigChannel.reset(new Channel(&_loop,_signalfd));
	_sigChannel->setReadCallback(boost::bind(&Service::onReadSignal,this));
	_sigChannel->enableReading();
	{
		AppendFile f(_pidFile);
		string pidnum = StringUtil::formateInteger(ProcessInfo::pid());
		f.append(pidnum.c_str(), pidnum.size());
	}
	_active = true;
	run();	
	_loop.loop();
	dispose();
}
void Service::version()
{
	cout<<"walle energy 1.0"<<endl;
}

void Service::onReadSignal()
 {
	struct signalfd_siginfo fdsi;
	ssize_t s = read(_signalfd, &fdsi, sizeof(struct signalfd_siginfo));
	if (s != sizeof(struct signalfd_siginfo)) {
		LOG_ERROR<<"read signal error size: "<<s
			<<"should be :"<<sizeof(struct signalfd_siginfo);
	}
	if (fdsi.ssi_signo == SIGINT) {
		LOG_DEBUG<<"SIGINT signal to stop";
		stop();
	}
	
	if (fdsi.ssi_signo == SIGKILL) {
		LOG_DEBUG<<"kill signal to stop";
			stop();
	} 

 }
}
}

