#include <walle/wlevel/db.h>
#include <string>
#include <iostream>
using namespace std;
using namespace walle::wlevel;

int main ()
{
	DB* pdb= NULL;
	Options options;
	options.create_if_missing = true;
	Status status = DB::Open(options, "/tmp/testdb", &pdb);
	if(!status.ok()) {
		cout<<"open db error"<<endl;
	}

	string r;
	status = pdb->Put(WriteOptions(), "jack", "10");
	status = pdb->Put(WriteOptions(), "Hr", "mm");
	status = pdb->Get(ReadOptions(), "jack",&r);
	cout<<r <<": "<<r<<endl;
	status = pdb->Get(ReadOptions(), "Hr",&r);
	cout<<r <<": "<<r<<endl;
	status = pdb->Delete(WriteOptions(), "jack");
	delete pdb;

}
