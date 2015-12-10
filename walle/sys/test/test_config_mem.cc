#include <walle/sys/wallesys.h>
#include <string.h>
#include <string>
#include <iostream>

using namespace std;
using namespace walle::sys;
int main(int argc, char** argv)
{
	(void)argc;
	ReadSmallFile r(argv[1]);
	string configdata;
	r.read(&configdata,4096);
	Config conf;
	bool ret = conf.loadFromArray(configdata.c_str(),configdata.size());
	if(!ret) {
	cout<<"parse config error\n";
	}
	cout<<configdata<<endl;
	
	cout<< conf.toString()<<endl;



}
