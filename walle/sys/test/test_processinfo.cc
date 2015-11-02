#include <walle/sys/wallesys.h>
#include <iostream>
using namespace std;
using namespace walle::ProcessInfo;
int main()
{
	cout<<hostname()<<endl;
	cout<<procname()<<endl;
	cout<<pid()<<endl;
	cout<<procStat()<<endl;
	cout<<procStatus()<<endl;
	cout<<exePath()<<endl;
	cout<<euid()<<endl;
	cout<<uid()<<endl;
	cout<<startTime().toDateTime()<<endl;
}
