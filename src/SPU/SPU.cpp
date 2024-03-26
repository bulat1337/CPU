#include <stdio.h>

#include "SPU_additional.h"
#include "SPU.h"

#define FILE_PTR_CHECK(file_ptr)                                    \
    if(file_ptr == NULL)                                            \
    {                                                               \
        CPU_LOG("\nERROR: Unable to open "#file_ptr"\n");           \
        return SPU_UNABLE_TO_OPEN_FILE;								\
	}

error_t execute(const char *bin_file, const char *config_file)
{
	error_t error_code = SPU_ALL_GOOD;

	WITH_OPEN
	(
		"execution_result.txt", "w", exe_result,

		WITH_OPEN
		(
			bin_file, "rb", bin_file_ptr,

			WITH_OPEN
			(
				config_file, "r", config_file_ptr,

				CALL(process(bin_file_ptr, config_file_ptr, exe_result));
			)
		)
	)

	return error_code;
}
