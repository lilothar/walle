#include <stdio.h>
#include <fstream>
#include <string.h>

using namespace std;
char * toLower(char* buff, size_t len);

int main()
{
	ifstream f("/etc/issue",ios::out);
	char buff[1024] = "";
	f.getline(buff, 1023);
	if(strcasestr(buff,"ubuntu")) {
		printf("ubuntu");
	} else if(strcasestr(buff,"centos")) {
		printf("centos");
	} else if(strcasestr(buff,"redhat") ||
		strcasestr(buff,"red hat")) {
		printf("redhat");
	} else if(strcasestr(buff,"debian")) {
		printf("debian");	
	} else if(strcasestr(buff,"suse")) {
		printf("suse");	
	} else if(strcasestr(buff,"mint")) {
		printf("ubuntu");	
	} else {
		printf("unknow");	
	} 
	
	return 0;
}
