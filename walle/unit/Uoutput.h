#ifndef U_OUTPUT_H_
#define U_OUTPUT_H_
#include <string>


#define CPPTEST_UNUSED(x) (void)x

namespace unit{

	class Source;
	class Time;
	
	class Output
	{
	public:

		virtual ~Output() {}
		

		virtual void initialize(int tests)
		{
			CPPTEST_UNUSED(tests);
		}
		
		virtual void finished(int tests, const Time& time)
		{
			CPPTEST_UNUSED(tests);
			CPPTEST_UNUSED(time);
		}

		virtual void suite_start(int tests, const std::string& name)
		{
			CPPTEST_UNUSED(tests);
			CPPTEST_UNUSED(name);
		}
		

		virtual void suite_end(int tests, const std::string& name,
							   const Time& time)
		{
			CPPTEST_UNUSED(tests);
			CPPTEST_UNUSED(name);
			CPPTEST_UNUSED(time);
		}
		

		virtual void test_start(const std::string& name)
		{
			CPPTEST_UNUSED(name);
		}
		

		virtual void test_end(const std::string& name, bool ok,
							  const Time& time)
		{
			CPPTEST_UNUSED(name);
			CPPTEST_UNUSED(ok);
			CPPTEST_UNUSED(time);
		}
		
		virtual void assertment(const Source& s)
		{
			CPPTEST_UNUSED(s);
		}

	protected:

		Output() {}
		
	private:
		Output(const Output&);
		Output& operator=(const Output&);
	};
}

#endif
