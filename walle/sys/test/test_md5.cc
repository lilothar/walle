#include <iostream>
#include <walle/sys/wallesys.h>

int main(int argc, char **argv)
{
	if(argc < 2) {
		return -1;
	}
	walle::util::MD5 md5;
	string result;
	result.clear();
	walle::sys::ReadSmallFile rf(argv[1]);
	do {
		rf.read(&result,1024*128);
		md5.update(result);
	}while(result.length() == 1024*128);
	string r = walle::sys::StringUtil::hexEncode(md5.digest(),16);
	cout<<r<<endl;
	string w = walle::sys::StringUtil::hexDecode((const unsigned char*)r.c_str(), r.length());
	//cout<<md5.toString()<<endl;
	string r2 = walle::sys::StringUtil::hexEncode((const unsigned char*)w.c_str(),w.length());
	cout<<r2<<endl;
	return 0;
}
