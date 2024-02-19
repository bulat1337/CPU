#include "SPU.h"

int main()
{
	error_t error = execute("byte_code.bin");
	

	if(error != SPU_ALL_GOOD)
	{
		fprintf(stderr, "Execution ended with an error\n");
	}

	return 0;
}
