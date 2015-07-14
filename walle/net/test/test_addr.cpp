#include <walle/net/Addrinet.h>
#include <iostream>
using namespace std;
int main()
{
	const char* addr1 = "192.168.12.3:121";
	const char* addr2 = "10.2.4.19";
	walle::net::AddrInet iaddr(addr1);
	walle::net::AddrInet iaddr1(addr2,10000);
	cout<<iaddr.toString()<<endl;
	cout<<iaddr1.toString()<<endl;
	
}
