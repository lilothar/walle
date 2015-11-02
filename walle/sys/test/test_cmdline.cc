#include <walle/sys/wallesys.h>
#include <iostream>
using namespace std;
int main(int argc, char**argv) 
{
	using namespace walle::sys;
	parser p;
	p.add<string>("host", 'h', "host name", true, "");
	p.add<int>("port", 'p', "port number", false, 80,range(1, 65535));
	p.add<string>("type", 't', "protocol type", false, "http", oneof<string>("http", "https", "ssh", "ftp"));
	p.add("gzip", 'g', "gzip when transfer");
	p.parse_check(argc, argv);

	cout << p.get<string>("type") << "://"
 	<< p.get<string>("host") << ":"
 	<< p.get<int>("port") << endl;
	if(p.exist("gzip")) {
		cout<<"compress: gzip enabled"<<endl;
	} else {
		cout<<"compress: gzip disabled"<<endl;	
	}
	return 0;
}
