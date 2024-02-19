#include <stdio.h>

#include "assembler.h"
#include "assembler_additional.h"

error_t compile(const char *file_name)
{
	struct Parse_human_code_result parse_human_code_result = parse_human_code(file_name);

	if(parse_human_code_result.error_code != ASM_ALL_GOOD)
	{
		return parse_human_code_result.error_code;
	}
	struct Cmds_process_result cmds_process_result =
		cmds_process(parse_human_code_result.strings,
		             parse_human_code_result.amount_of_lines);

	struct Buf_w_carriage_n_len byte_code_buf_w_info =
		arrange_labels(cmds_process_result).second_arg.buf_w_info;

	FILE *byte_code = fopen("byte_code.bin", "wb");
	if(byte_code == NULL)
	{
		CPU_LOG("Unable to open byte_code.bin");

		return ASM_UNABLE_TO_OPEN_FILE;
	}

	fwrite(byte_code_buf_w_info.buf, sizeof(char), byte_code_buf_w_info.length, byte_code);

	CPU_LOG("Byte_code size: %lu\n", byte_code_buf_w_info.length);

	return ASM_ALL_GOOD;
}
