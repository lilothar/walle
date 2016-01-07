#include <stdio.h>
#include <fstream>
#include <string.h>

using namespace std;
char * toLower(char* buff, size_t len);

int main()
{
	ifstream f("/etc/issue",ios::out);
	char buff[1024] = "";
	int len =0;
	do {
		memset(buff, 0, 1024);
		f.getline(buff, 1023);
		len = strlen(buff);
		if(strcasestr(buff,"ubuntu")) {
			fprintf(stdout,"ubuntu");
			return 0;
		} else if(strcasestr(buff,"centos")) {
			printf("centos");
			return 0;
		} else if(strcasestr(buff,"redhat") ||
			strcasestr(buff,"red hat")) {
			printf("redhat");
			return 0;
		} else if(strcasestr(buff,"debian")) {
			printf("debian");
			return 0;
		} else if(strcasestr(buff,"suse")) {
			printf("suse");	
			return 0;
		} else if(strcasestr(buff,"mint")) {
			printf("ubuntu");
			return 0;
		} else {
			continue;
		}
	}while(!f.eof());
	printf("unknow");
	return 0;
}
