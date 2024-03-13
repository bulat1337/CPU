#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"

int main(int argc, const char *argv[])
{
	if(argc > 2)
	{
		fprintf(stderr, "ERROR: invalid amount of main function arguments(argc = %d)\n", argc);

		return EXIT_FAILURE;
	}
	error_t error = compile(argv[1]);

	if(error != ASM_ALL_GOOD)
	{
		printf("error: %d", error);
	}

	return 0;
}
