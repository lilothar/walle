#ifndef U_HTMLOUTPUT_H_
#define U_HTMLOUTPUT_H_


#include <iostream>
#include <string>

#include "Ucollectoroutput.h"

namespace unit
{

	class HtmlOutput : public CollectorOutput{
	public:
		void generate(std::ostream& os, bool incl_ok_tests = true, 
					  const std::string& name = "");
		
	private:
		struct SuiteRow;
		struct TestRow;
		struct TestSuiteRow;
		struct TestResult;
		struct TestResultAll;
		struct SuiteTestResult;

		friend struct TestSuiteRow;
	};
	
}

#endif
