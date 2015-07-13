
#include <algorithm>
#include "Utextoutput.h"
#include "Utime.h"
#include "Uutils.h"

using namespace std;

namespace unit
{


		struct ShowSource
		{
			ostream& _stream;
			ShowSource(ostream& stream) : _stream(stream) {}
			void operator()(const Source& s)
			{
				_stream << "\tTest:    " << s.test()    << endl
						<< "\tSuite:   " << s.suite()   << endl
						<< "\tFile:    " << s.file()    << endl
						<< "\tLine:    " << s.line()    << endl
						<< "\tMessage: " << s.message() << endl << endl;
						
			}
		};
	

	TextOutput::TextOutput(Mode mode, ostream& stream)
	:   _mode(mode),
		_stream(stream),
		_total_errors(0)
	{}
	
	void
	TextOutput::finished(int tests, const Time& time)
	{
		_stream	<< "Total: " << tests << " tests, "
				<< correct(tests, _total_errors) << "% correct"
				<< " in " << time << " seconds" << endl;
	}
	
	void
	TextOutput::suite_start(int tests, const string& name)
	{
		if (tests > 0)
		{
			_suite_name  = name;
			_suite_tests = _suite_errors = 0;
			_suite_total_tests = tests;
			_suite_error_list.clear();
			
			_stream	<< _suite_name << ": "
					<< "0/" << _suite_total_tests
					<< "\r" << flush;
		}
	}
	
	void
	TextOutput::suite_end(int tests, const string& name, const Time& time)
	{
		if (tests > 0)
		{
			_stream	<< name << ": " << tests << "/" << tests << ", "
					<< correct(tests, _suite_errors) << "% correct"
					<< " in " << time << " seconds" << endl;
            			
			if (_mode == Verbose && _suite_errors)
				for_each(_suite_error_list.begin(), _suite_error_list.end(),
						 ShowSource(_stream));
			
			_total_errors += _suite_errors;
		}
	}
	
	void
	TextOutput::test_end(const string&, bool ok, const Time&)
	{
		_stream	<< _suite_name << ": "
				<< ++_suite_tests << "/" << _suite_total_tests
				<< "\r" << flush;
		if (!ok)
			++_suite_errors;
	}
	
	void
	TextOutput::assertment(const Source& s)
	{
		_suite_error_list.push_back(s);
	}
	
} 

