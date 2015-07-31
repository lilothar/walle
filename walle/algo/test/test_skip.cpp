#include<walle/algo/wallealgo.h>
#include <iostream>

using namespace std;
int main ()
{
	walle::algorithm::skiplist<int, int> sl;

	sl.insert(4);
	sl.insert(5);
	sl.insert(1);
	sl.insert(2);
	sl.insert(9);
		sl.insert(2);	sl.insert(2);
		
		walle::algorithm::skiplist<int, int>::iterator itr = sl.begin();
		cout<<sl.size()<<endl;
		while(itr != sl.end()) {
			cout<<*itr++<<endl;
		}
	
	return 0;
}
