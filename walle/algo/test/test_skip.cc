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
		cout<<"@@@@@@@@@@@@@@@@@@"<<endl;
		sl.remove(5);
		itr = sl.begin();
		cout<<sl.size()<<endl;
		while(itr != sl.end()) {
			cout<<*itr++<<endl;
		}
	
		cout<<"1224"<<endl;
		itr =sl.find_lower(5);
		while(itr != sl.rend()) {
			cout<<*itr--<<endl;
		}

		cout<<"2 2 4 9"<<endl;
		itr =sl.find_upper(2);
		while(itr != sl.end()) {
			cout<<*itr++<<endl;
		}

		itr = sl.find(4);
		cout<<*itr<<endl;
	return 0;
}
