
#include "Usource.h"

using namespace std;

namespace unit
{
	Source::Source()
	:	_line(0)
	{}
	

	Source::Source(const char* spfile, unsigned int ipline, const char* spmsg)
	:	_line(ipline),
		_file(spfile ? spfile : ""),
		_msg(spmsg ? spmsg : "")
	{}
	
	const string&
	Source::file() const 	
	{
		return _file;
	}
	

	unsigned int
	Source::line() const
	{
		return _line;
	}
	

	const string&
	Source::message() const
	{
		return _msg;
	}
	
	const string&
	Source::suite() const
	{
		return _suite;
	}


	const string&
	Source::test() const
	{
		return _test;
	}
}


