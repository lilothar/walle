#include "Ucollectoroutput.h"

using namespace std;

namespace unit
{
	CollectorOutput::TestInfo::TestInfo(const string name)
	:	_name(name)
	{}

	CollectorOutput::SuiteInfo::SuiteInfo(const string& name, int tests)
	:	_name(name),
		_errors(0)
	{
		_tests.reserve(tests);
	}
	
	/// Constructs a collector object.
	///
	CollectorOutput::CollectorOutput()
	:	Output(),
		_total_errors(0)
	{}
	
	void
	CollectorOutput::finished(int tests, const Time& time)
	{
		_total_tests = tests;
		_total_time  = time;
	}
	
	void
	CollectorOutput::suite_start(int tests, const string& name)
	{
		if (tests > 0)
		{
			_suites.push_back(SuiteInfo(name, tests));
			_cur_suite = &_suites.back();
		}
	}
	
	void
	CollectorOutput::suite_end(int tests, const string&, const Time& time)
	{
		if (tests > 0)
		{
			_cur_suite->_time = time;
			_total_errors += _cur_suite->_errors;
		}
	}
	
	void
	CollectorOutput::test_start(const string& name)
	{
		_cur_suite->_tests.push_back(TestInfo(name));
		_cur_test = &_cur_suite->_tests.back();
	}
	
	void
	CollectorOutput::test_end(const string&, bool ok, const Time& time)
	{
		if (!(_cur_test->_success = ok))
			++_cur_suite->_errors;
		_cur_test->_time    = time;
	}
	
	void
	CollectorOutput::assertment(const Source& s)
	{
		_cur_test->_sources.push_back(s);
	}
	
}

