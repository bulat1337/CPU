#include <stdio.h>
#include <stdlib.h>

#include "SPU.h"

int main(int argc, char *argv[])
{
	if(argc > 2)
	{
		fprintf(stderr, "ERROR: invalid amount of main function arguments(argc = %d)\n", argc);

		return EXIT_FAILURE;
	}

	error_t error_code = execute(argv[1]);

	if(error_code != SPU_ALL_GOOD)
	{
		fprintf(stderr, "Execution ended with an error\n");
	}

	return 0;
}
