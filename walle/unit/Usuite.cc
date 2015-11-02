#include <algorithm>
#include <cassert>
#include <cstring>
#include <functional>
#include <numeric>

#include "Uoutput.h"
#include "Usource.h"
#include "Usuite.h"

using namespace std;

namespace unit
{
	template <class FwdIter>
	void
	destroy_range(FwdIter first, FwdIter last)
	{
		while (first != last)
			delete *first++;
	}

	Suite::Suite():_cur_test(0), 
		_output(0),
		_success(true)
	{}
	

	Suite::~Suite()
	{
		destroy_range(_suites.begin(), _suites.end());
	}
	

	bool
	Suite::run(Output& output, bool cont_after_fail)
	{
		int ntests = total_tests();
		output.initialize(ntests);
		do_run(&output, cont_after_fail);
		output.finished(ntests, total_time(true));
		return _success;
	}

	void
	Suite::add(auto_ptr<Suite> suite)
	{
		_suites.push_back(suite.release());
	}
	

	void
	Suite::register_test(Func func, const string& name)
	{
		string::size_type pos = name.find_first_of(':');
		assert(!name.empty() && name[pos + 1] == ':' && name[pos + 2] != '\0');
		
		_name.assign(name, 0, pos);
		_tests.push_back(Data(func, name.substr(pos + 2)));
	}
	

	void
	Suite::assertment(Source s)
	{
		s._suite = _name;
		s._test  = *_cur_test;
		_output->assertment(s);
		_result = _success = false;
	}
	
	struct Suite::ExecTests
	{
		Suite& _suite;
		
		ExecTests(Suite& s) : _suite(s) {}
		
		void operator()(Data& data)
		{
			_suite._cur_test = &data._name;
			_suite._result = true; 
			_suite._output->test_start(data._name);
			
			_suite.setup();
			Time start(Time::current());
			
			try
			{
				(_suite.*data._func)();
			} catch (...) {
				_suite._result = _suite._success = false;
			}
			Time end(Time::current());
			_suite.tear_down();
			
			data._time = end - start;
			_suite._output->test_end(data._name, _suite._result, data._time);
		}
	};


	struct Suite::DoRun
	{
		bool	_continue;
		Output* _output;
		
		DoRun(Output* output, bool cont) : _continue(cont), _output(output) {}
		void operator()(Suite* suite) { suite->do_run(_output, _continue); }
	};


	void
	Suite::do_run(Output* os, bool cont_after_fail)
	{
		_continue = cont_after_fail;
		_output = os;
		
		_output->suite_start((int)_tests.size(), _name);
		for_each(_tests.begin(), _tests.end(), ExecTests(*this));
		_output->suite_end((int)_tests.size(), _name, total_time(false));

		for_each(_suites.begin(), _suites.end(), DoRun(_output, _continue));


		Suites::const_iterator iter = _suites.begin();
		while (iter != _suites.end())
		{
			if (!(*iter)->_success)
			{
				_success = false;
				break;
			}
			iter++;
		}
	}


	struct Suite::SubSuiteTests
	{
		int operator()(size_t value, const Suite* s) const
		{
			return (int)(value + s->total_tests());
		}
	};
	

	int
	Suite::total_tests() const
	{
		return accumulate(_suites.begin(), _suites.end(),
						  (int)_tests.size(), SubSuiteTests());
	}
	

	struct Suite::SuiteTime
	{
		Time operator()(const Time& time, const Data& data)
		{
			return time + data._time;
		}
	};
	

	struct Suite::SubSuiteTime
	{
		Time operator()(Time time, const Suite* s) const
		{
			return time + s->total_time(true);
		}
	};
	

	Time
	Suite::total_time(bool recursive) const
	{
		Time time = accumulate(_tests.begin(), _tests.end(),
							   Time(), SuiteTime());
		
		return !recursive ? time : accumulate(_suites.begin(), _suites.end(),
											  time, SubSuiteTime());
	}
	
} 

