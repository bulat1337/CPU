#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SPU.h"
#include "drivers.h"

int main(const int argc, const char *argv[])
{
	if(argc > 3)
	{
		fprintf(stderr, "ERROR: invalid amount of main function arguments(argc = %d)\n", argc);

		return EXIT_FAILURE;
	}

	void (*driver)(VM *, char *, FILE *) = &terminal_draw;

	error_t error_code = execute(argv[1], argv[2], driver);

	if(error_code != SPU_ALL_GOOD)
	{
		fprintf(stderr, "Execution ended with an error\n");
	}

	return 0;
}
