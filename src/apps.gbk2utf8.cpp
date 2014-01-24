#include <stdio.h>
#include "fmts.utf8.h"

int main()
{
	const char * pocode = "GBK";
	char  input[] = "Œ“ «À≠";
	char * output = NULL;
	utf8(pocode, input, &output);
	printf("input:%s\n", input);
	printf("output:%s\n", output);
	return 0;
}
