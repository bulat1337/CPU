#include <stdio.h>

#include "assembler.h"

int main()
{
	error_t error = compile("human_code");

	if(error != ASM_ALL_GOOD)
	{
		printf("error: %d", error);
	}

	return 0;
}
