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
#include <walle/sys/wallesys.h>


using namespace walle::sys;
using namespace std;
namespace walle {

namespace net {

namespace detail{

int createSignalfd(int fd, sigset_t *mask)
{

	if(sigprocmask(SIG_BLOCK, mask, NULL) == -1) {
		LOG_ERROR<<"sigprocmask error errno:"<<errno;
		return -1;
	}
	
	int refd = signalfd(fd, mask ,SFD_NONBLOCK | SFD_CLOEXEC);
	
	return refd;
	
}
AsyncLogging* g_log = NULL;

void gAlogOutput(const char* buff, size_t len)
{
	g_log->append(buff, len);
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
	Filesystem::deleteFile(_pidFile);
	_loop->quit();
}
void Service::setParseOption()
{
	_args.add("version", 'v', "show version");
	_args.add<string>("config", 'c', "config file path", false,"./service.conf");
	_args.add<string>("command", 'k', "user cmd", false);
}

int Service::main(int argc, char* argv[])
{
	
	_isdaemon = false;
	
	setParseOption();
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

	for(int i=0;i< NOFILE;++i) {
		close(i);
	}
	
	int fd = open("/dev/null",O_RDWR| O_CLOEXEC );
	assert( fd == 0 );
	fd = dup2(0,1);
	assert( fd == 1 );
	fd = dup2(1,2);
	assert( fd == 2 ); 
	UNUSED(fd);
	_workdir = _conf.getString("global","service_work_dir");
	if(!_workdir.empty()) {
		int ret = chdir(_workdir.c_str());
		UNUSED(ret);
		
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
	if(initLogConf() == false) {
		cout<<"log load config error"<<endl;	
		exit(0);
	}
	
	if(Filesystem::fileExist(_pidFile) ){
			cout<<"already running"<<endl;	
			exit(0);
	}

	if(!Filesystem::fileExist(_workdir)) {
		cout<<"working dir not exist"<<endl;
		exit(0);
	}

	if(_isdaemon ) {
		runDaemon();
	}

	EventLoop loop;
	_loop = &loop;
	

	_signalfd = detail::createSignalfd(-1, &_sigmask);
	if(_signalfd < 3) {
		cerr<<"create signal fd error"<<endl;
		exit(0);
	}
	
	LOG_INFO<<"signal fd : "<<_signalfd;
	_sigChannel.reset(new Channel(_loop,_signalfd));
	_sigChannel->setReadCallback(std::bind(&Service::onReadSignal,this,std::_1));
	_sigChannel->enableReading();
	Logger::setLogLevel(_loglevel);
	/*
	AsyncLogging alog(_logbase,_rollsize,_interval);
	detail::g_log =&alog;
	Logger::setLogLevel(_loglevel);
	Logger::setOutput(detail::gAlogOutput);
	alog.waitStart();
	*/
	{
		
		AppendFile f(_pidFile);
		string pidnum = StringUtil::formateInteger(ProcessInfo::pid());
		f.append(pidnum.c_str(), pidnum.size());
	}
	//init log
	
	_active = true;
	int ret = run();
	if(ret == 0 ) {
		_loop->loop();
	}

	//alog.stop();
	dispose();
}
bool Service::initLogConf()
{
	string logdir = _conf.getString("log","service_log_dir");
	string logprefix = _conf.getString("log","log_name_prefix");
	 _interval = _conf.getInt("log","log_flush_interval",3);
	 _rollsize = _conf.getInt("log","log_roll_size");	
	string level = _conf.getString("log","log_level","info");
	if(logdir.empty()||logprefix.empty()) {
		return false;
	}
	if(logdir[0] !='/') {
		return false;
	}
	if(logprefix[0] == '/') {
		return false;
	}
	if(_rollsize < 1024*1024 ) {
		_rollsize = 10*1024*1024;
	}
	_logbase = logdir;
	if(_logbase[_logbase.size()-1] != '/' ) {
		_logbase +="/";
		_logbase += logprefix;
	} else {
		_logbase += logprefix;
	}
	if(level == "trace" || level == "TRACE") {
		_loglevel = Logger::TRACE;
	} else if(level == "debug" || level == "DEBUG") {
		_loglevel = Logger::DEBUG;
	}else if(level == "info" || level == "INFO") {
		_loglevel = Logger::INFO;
	}else if(level == "WARN" || level == "warn") {
		_loglevel = Logger::WARN;
	}else if(level == "error" || level == "ERROR") {
		_loglevel = Logger::ERROR;
	}else if(level == "fatal" || level == "FATAL") {
		_loglevel = Logger::FATAL;
	} else {
		return false;
	}
	return true;
}

void Service::version()
{
	cout<<"walle energy 1.0"<<endl;
}

void Service::onReadSignal(Time t)
{
	(void)t;
	struct signalfd_siginfo fdsi;
	memset(&fdsi,0,sizeof(fdsi));
	ssize_t s = read(_signalfd, &fdsi, sizeof(struct signalfd_siginfo));
	if (s != sizeof(struct signalfd_siginfo)) {
		LOG_ERROR<<"read signal error size: "<<s
			<<"should be :"<<sizeof(struct signalfd_siginfo);
		return stop();
	}
	if (fdsi.ssi_signo == SIGINT) {
		LOG_DEBUG<<"SIGINT signal to stop";
		stop();
	}
	
	if (fdsi.ssi_signo == SIGKILL) {
		LOG_DEBUG<<"kill signal to stop";
			stop();
	}
	LOG_WARN<<"recv signal: "<<fdsi.ssi_signo;

 }
}
}

