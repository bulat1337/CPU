#include <stdio.h>

#include "SPU_additional.h"
#include "SPU.h"

#define FILE_PTR_CHECK(file_ptr)                                    \
    if(file_ptr == NULL)                                            \
    {                                                               \
        CPU_LOG("\nERROR: Unable to open "#file_ptr"\n");           \
        return SPU_UNABLE_TO_OPEN_FILE;								\
    }

error_t execute(const char *file_name)
{
	error_t error_code = SPU_ALL_GOOD;

	WITH_OPEN
	(
		"execution_result.txt", "w", exe_result,

		WITH_OPEN
		(
			file_name, "rb", byte_code_file,

			error_code = process(byte_code_file, exe_result);
			if(error_code != SPU_ALL_GOOD)
			{
				CPU_LOG("\nERROR: code_%d\n", error_code);

				return error_code;
			}
		)
	)

	return error_code;
}
