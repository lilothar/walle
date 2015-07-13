#ifndef U_COLLECTOROUTPUT_H_
#define U_COLLECTOROUTPUT_H_

#include <list>
#include <string>
#include <vector>

#include "Uoutput.h"
#include "Usource.h"
#include "Utime.h"

namespace unit
{

	class CollectorOutput : public Output
	{
	public:
		virtual void finished(int tests, const Time& time);
		virtual void suite_start(int tests, const std::string& name);
		virtual void suite_end(int tests, const std::string& name,
							   const Time& time);
		virtual void test_start(const std::string& name);
		virtual void test_end(const std::string& name, bool ok,
							  const Time& time);
		virtual void assertment(const Source& s);
		
	protected:
		struct OutputSuiteInfo;
		struct OutputTestInfo;
		struct OutputErrorTestInfo;
		
		typedef std::list<Source> Sources;
		
		struct TestInfo
		{
            std::string _name;
			Time		_time;
			
			bool		_success : 1;
			Sources		_sources;
			
			explicit TestInfo(const std::string name);
		};
		
		typedef std::vector<TestInfo> Tests;
		
		struct SuiteInfo
		{
			std::string	_name;
			int			_errors;
			Tests		_tests;
			Time		_time;
			
			SuiteInfo(const std::string& name, int tests);
		};
		
		typedef std::list<SuiteInfo> Suites;
		
		Suites			_suites;
		int				_total_errors;
		int				_total_tests;
		Time			_total_time;
		
		CollectorOutput();
		
	private:
		SuiteInfo*		_cur_suite;
		TestInfo*		_cur_test;
	};
	
}
#endif