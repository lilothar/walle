#include<walle/sys/Time.h>
#include <iostream>
using namespace std;
using walle::sys::Time;
int main ()

{
	const char* date1 = "2015-07-022 17:23:3.2343";
	const char* date2 = "1990/09/07/10/2/3/2343";
	string str1 = Time::dateString(date1, "- :.").toDateTime();

	cout<<"date1:"<<str1<<endl;
	string str2 = Time::dateString(date2, "/").toDateTime();
	cout<<"date2:"<<str2<<endl;
	
	
}
