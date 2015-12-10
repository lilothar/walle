#include <unistd.h>
#include <stdio.h>
int main()
{
	int s = getpagesize();
	printf("%d",s);
	return 0;
}
