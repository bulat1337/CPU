#include <stdio.h>

#include "SPU_additional.h"
#include "SPU.h"

error_t execute(const char *bin_file, const char *config_file,
				void (*driver)(VM *, char *, FILE *))
{
	error_t error_code = SPU_ALL_GOOD;

	WITH_OPEN
	(
		"execution_result.txt", "w", exe_result,

		WITH_OPEN
		(
			bin_file, "rb", bin_file_ptr,

			CALL(process(bin_file_ptr, config_file, exe_result, driver));
		)
	)

	return error_code;
}
