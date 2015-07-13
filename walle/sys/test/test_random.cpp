#include <walle/sys/wallesys.h>
#include <iostream>
using namespace std;
int main ()
{
	walle::util::Random R(time(NULL));
	for(int i =0; i<1000;i++) {
		cout<<R.next()<<endl;
	}
	return 0;
}
