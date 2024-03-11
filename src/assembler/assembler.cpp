#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "assembler_additional.h"

/**
 * @def CHECK_ERROR(result)
 * @brief Macro to check if an error occurred during a function call.
 * @param result The result structure to check for errors.
 * @return Returns the error code if an error occurred, otherwise continues execution.
 */
#define CHECK_ERROR\
	if(error_code != ASM_ALL_GOOD)\
		return error_code;

/**
 * @def REDUCED_BYTE_CODE
 * @brief Macro representing the reduced byte code after buffer size reduction.
 */
#define REDUCED_BYTE_CODE\
	reduce_buffer_size_result.second_arg.buf_w_info

/**
 * @def FILE_PTR_CHECK(file_ptr)
 * @brief Macro to check if a file pointer is valid.
 * @param file_ptr The file pointer to check.
 * @return Returns ASM_UNABLE_TO_OPEN_FILE if the file pointer is NULL.
 */
#define FILE_PTR_CHECK(file_ptr)									\
	if(file_ptr == NULL)											\
	{																\
		CPU_LOG("\nERROR: Unable to open "#file_ptr"\n");			\
		return ASM_UNABLE_TO_OPEN_FILE;								\
	}

#define CALL(...)				\
	error_code = __VA_ARGS__;	\
	CHECK_ERROR;

error_t compile(const char *file_name)
{
	Compile_manager manager = {};
	error_t error_code = ASM_ALL_GOOD;

	CALL(parse_human_code(&manager, file_name))

	CALL(cmds_process(&manager))

	CALL(arrange_labels(&manager))

	CALL(reduce_buffer_size(&(manager.byte_code)))

	CALL(create_bin(&manager, file_name))

	manager_dtor(&manager);

	return ASM_ALL_GOOD;
}

#undef CHECK_ERROR
#undef FILE_PTR_CHECK
#undef REDUCED_BYTE_CODE
