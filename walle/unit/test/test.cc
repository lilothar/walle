#include <cstdlib>
#include <cstring>
#include <iostream>
#include <walle/unit/Utest.h>
using namespace std;
int addnum(int a, int b);

int addnum(int a, int b)
{
	return a+b;
}
class FailTestSuite : public unit::Suite
{
public:
	FailTestSuite()
	{
		TEST_ADD(FailTestSuite::success)
		TEST_ADD(FailTestSuite::always_fail)
	
	}
	
private:
	void success() {}
	
	void always_fail()
	{
		// This will always fail
		//
		//TEST_FAIL("unconditional fail");
		TEST_FAIL(0);
	}
};

class CompareTestSuite : public unit::Suite
{
public:
	CompareTestSuite()
	{
		//TEST_ADD(CompareTestSuite::success)
		TEST_ADD(CompareTestSuite::compare);
		TEST_ADD(CompareTestSuite::delta_compare);
		TEST_ADD(CompareTestSuite::testaddnum);
	}
	
private:
	void success() {}
	
	void compare()
	{
		// Will succeed since the expression evaluates to true
		
		TEST_ASSERT(1 + 1 == 2)
		
		// Will fail since the expression evaluates to false
		//
		//TEST_ASSERT(0 == 1);
		
	}
	void testaddnum()
	{
		TEST_ASSERT_EQUALS(2,addnum(1,1));
	}
	
	void delta_compare()
	{
		// Will succeed since the expression evaluates to true
		//
		TEST_ASSERT_DELTA(0.5, 0.7, 0.3);
		
		// Will fail since the expression evaluates to false
		//
		//TEST_ASSERT_DELTA(0.5, 0.7, 0.1);
	}
};

enum OutputType
{
	Compiler,
	Html,
	TextTerse,
	TextVerbose
};

static void
usage()
{
	cout << "usage: mytest [MODE]\n"
		 << "where MODE may be one of:\n"
		 << "  --compiler\n"
		 << "  --html\n"
		 << "  --text-terse (default)\n"
		 << "  --text-verbose\n";
	exit(0);
}

static auto_ptr<unit::Output>
cmdline(int argc, char* argv[])
{
	if (argc > 2)
		usage(); // will not return
	
	unit::Output* output = 0;
	
	if (argc == 1)
		output = new unit::TextOutput(unit::TextOutput::Verbose);
	else
	{
		const char* arg = argv[1];
		if (strcmp(arg, "--compiler") == 0)
			output = new unit::CompilerOutput;
		else if (strcmp(arg, "--html") == 0)
			output =  new unit::HtmlOutput;
		else if (strcmp(arg, "--text-terse") == 0)
			output = new unit::TextOutput(unit::TextOutput::Terse);
		else if (strcmp(arg, "--text-verbose") == 0)
			output = new unit::TextOutput(unit::TextOutput::Verbose);
		else
		{
			cout << "invalid commandline argument: " << arg << endl;
			usage(); // will not return
		}
	}
	
	return auto_ptr<unit::Output>(output);
}

int
main(int argc, char* argv[])
{
	try
	{
		// Demonstrates the ability to use multiple test suites
		//
		unit::Suite ts;
		ts.add(auto_ptr<unit::Suite>(new FailTestSuite));
		ts.add(auto_ptr<unit::Suite>(new CompareTestSuite));

		// Run the tests
		//
		auto_ptr<unit::Output> output(cmdline(argc, argv));
		ts.run(*output, true);

		unit::HtmlOutput* const html = dynamic_cast<unit::HtmlOutput*>(output.get());
		if (html)
			html->generate(cout, true, "MyTest");
	}
	catch (...)
	{
		cout << "unexpected exception encountered\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


