#include <cstdlib>
#include <cstring>
#include <iostream>
#include <walle/unit/Utest.h>
#include <walle/sys/wallesys.h>
#include <string>

using namespace std;

class StringUtilUtest : public unit::Suite
{
public:
	StringUtilUtest()
	{
		TEST_ADD(StringUtilUtest::test_atoi)
		TEST_ADD(StringUtilUtest::test_atoix)
		TEST_ADD(StringUtilUtest::test_atof)
	
	}
	
private:
	
	void test_atoi() 
	{
		const char* numstr = "100";
			
		TEST_ASSERT_EQUALS(100,walle::sys::StringUtil::atoi(numstr,strlen(numstr)));
	}
	
	void test_atoix()
	{
		const char*numstr = "100k";
			
		TEST_ASSERT_EQUALS(100*1024,walle::sys::StringUtil::atoix(numstr,strlen(numstr)));
	}
	void test_atof()
	{
		const char*numstr = "100.93";
			
		TEST_ASSERT_EQUALS(100.93,walle::sys::StringUtil::atof(numstr,strlen(numstr)));
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
		ts.add(auto_ptr<unit::Suite>(new StringUtilUtest));
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




