#ifndef U_TEXT_OUTPUT_H_
#define U_TEXT_OUTPUT_H_
#include <iostream>
#include <list>

#include "Usource.h"
#include "Uoutput.h"

namespace unit{

	class TextOutput : public Output
	{
	public:

		enum Mode
		{
			Terse,
			Verbose			
		};
		
		TextOutput(Mode mode, std::ostream& stream = std::cout);
		
		virtual void finished(int tests, const Time& time);
		virtual void suite_start(int tests, const std::string& name);
		virtual void suite_end(int tests, const std::string& name,
							   const Time& time);
		virtual void test_end(const std::string& name, bool ok,
							  const Time& time);
		virtual void assertment(const Source& s);
		
	private:
		typedef std::list<Source> ErrorList;
		
		Mode 			_mode;
        std::ostream& 	_stream;
		ErrorList		_suite_error_list;
		std::string		_suite_name;
		int				_suite_errors;
		int				_suite_tests;
		int				_suite_total_tests;
		int				_total_errors;
	};

} 
#endif