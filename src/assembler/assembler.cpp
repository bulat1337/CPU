#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"
#include "assembler_additional.h"

/**
 * @def CHECK_ERROR(result)
 * @brief Macro to check if an error occurred during a function call.
 * @param result The result structure to check for errors.
 * @return Returns the error code if an error occurred, otherwise continues execution.
 */
#define CHECK_ERROR(result)\
	if(result.error_code != ASM_ALL_GOOD)\
		return result.error_code;

/**
 * @def FIXED_BYTE_CODE
 * @brief Macro representing the fixed byte code after label arrangement.
 */
#define FIXED_BYTE_CODE\
	arrange_labels_result.second_arg.buf_w_info

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

error_t compile(const char *file_name)
{
	struct Parse_human_code_result parse_human_code_result = parse_human_code(file_name);
	CHECK_ERROR(parse_human_code_result);


	struct Cmds_process_result cmds_process_result =
		cmds_process(parse_human_code_result.strings,
		             parse_human_code_result.amount_of_lines);

	CHECK_ERROR(cmds_process_result);


	free(parse_human_code_result.strings);

	return_t arrange_labels_result =
		arrange_labels(cmds_process_result);

	CHECK_ERROR(arrange_labels_result);


	free(parse_human_code_result.human_code_buffer.buf);
	free(cmds_process_result.jmp_poses_w_carriage.JMP_poses);
	free(cmds_process_result.labels_w_carriage.labels);

	CPU_LOG("Byte_code size: %lu * 8 bytes\n", FIXED_BYTE_CODE.length / 8);

	return_t reduce_buffer_size_result = reduce_buffer_size(FIXED_BYTE_CODE);
	CHECK_ERROR(reduce_buffer_size_result);

	FILE *byte_code = fopen("byte_code.bin", "wb");
	FILE_PTR_CHECK(byte_code);

	fwrite(REDUCED_BYTE_CODE.buf, sizeof(char), REDUCED_BYTE_CODE.length, byte_code);

	fclose(byte_code);

	free(REDUCED_BYTE_CODE.buf);



	return ASM_ALL_GOOD;
}

#undef CHECK_ERROR
#undef FIXED_BYTE_CODE
#undef FILE_PTR_CHECK
#undef REDUCED_BYTE_CODE
