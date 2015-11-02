#include<walle/sys/Time.h>
#include <iostream>
using namespace std;
using walle::sys::Time;
int main ()

{
	const char* date1 = "2015-07-022 17:23:3.2343";
	const char* date2 = "1990/09/07/10/2/3/2343";
	
	Time t1  = Time::dateString(date1, "- :.");
	Time t2 = Time::dateString(date2, "/");
		
	cout<<"****date1******"<<date1<<endl;
	cout<<"string: "<<date1<<endl;
	cout<<"sec: "<<t1.toSeconds()<<endl;
	cout<<"usec: "<<t1.toMicroSeconds()<<endl;
	cout<<"string trans: "<<t1.toDateTime()<<endl;
	cout<<"*********date2********"<<endl;
	cout<<"string: "<<date2<<endl;
	cout<<"sec: "<<t2.toSeconds()<<endl;
	cout<<"usec: "<<t2.toMicroSeconds()<<endl;
	cout<<"string trans: "<<t2.toDateTime()<<endl;

	return 0;
	
	
}
