#include <cstdlib>
#include <cstring>
#include <iostream>
#include <walle/unit/Utest.h>
#include <walle/sys/Config.h>
#include <string>

using namespace std;

class ConfigUtest : public unit::Suite
{
public:
	ConfigUtest(const string &path):_path(path)
	{
		TEST_ADD(ConfigUtest::testLoad)
		TEST_ADD(ConfigUtest::testSection)
		TEST_ADD(ConfigUtest::testInt)
	
	}
	
private:
	walle::sys::Config _conf;
	string _path;
	void testLoad() 
	{
		
		TEST_ASSERT_EQUALS(true,_conf.load(_path));
	}
	
	void testSection()
	{
		vector < string > keys;
		keys.clear();
		_conf.getSectionKey("global", keys);
		TEST_ASSERT_EQUALS(2,keys.size());	
	}
	void testInt()
	{
		TEST_ASSERT_EQUALS(100,_conf.getInt("global","buffer_size",0));
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
		ts.add(auto_ptr<unit::Suite>(new ConfigUtest("./conf_test.conf")));

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




