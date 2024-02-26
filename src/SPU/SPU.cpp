#include <stdio.h>

#include "SPU_additional.h"
#include "SPU.h"

return_t execute(const char *file_name)
{
	return_t result =
	{
		.error_code = SPU_ALL_GOOD,
		.second_arg.file_ptr = fopen("exeution_result.txt", "w"),
	};

	if(result.second_arg.file_ptr == NULL)
	{
		CPU_LOG("Unable to open execution_result.txt\n");
		result.error_code = SPU_UNABLE_TO_OPEN_FILE;

		return result;
	}

	result.error_code = process(file_name);
	if(result.error_code != SPU_ALL_GOOD)
	{
		CPU_LOG("\nERROR: code_%d\n", result.error_code);

		return result;
	}

	fclose(result.second_arg.file_ptr);

	return result;
}
