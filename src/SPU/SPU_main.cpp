#include <stdio.h>

#include "SPU.h"

int main()
{
	return_t exe_result = execute("byte_code.bin");

	if(exe_result.error_code != SPU_ALL_GOOD)
	{
		fprintf(stderr, "Execution ended with an error\n");
	}

	return 0;
}
