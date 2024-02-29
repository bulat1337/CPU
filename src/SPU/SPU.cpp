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

	FILE *byte_code_file = fopen(file_name, "rb");

	if(byte_code_file == NULL)
	{
		CPU_LOG("Unable to open %s\n", file_name);
		result.error_code = SPU_UNABLE_TO_OPEN_FILE;

		return result;
	}

	if(result.second_arg.file_ptr == NULL)
	{
		CPU_LOG("Unable to open execution_result.txt\n");
		result.error_code = SPU_UNABLE_TO_OPEN_FILE;

		return result;
	}

	result.second_arg.file_ptr = stdout; // temp

	result.error_code = process(byte_code_file, result.second_arg.file_ptr);
	if(result.error_code != SPU_ALL_GOOD)
	{
		CPU_LOG("\nERROR: code_%d\n", result.error_code);

		return result;
	}

	fclose(byte_code_file);
	fclose(result.second_arg.file_ptr);

	return result;
}
