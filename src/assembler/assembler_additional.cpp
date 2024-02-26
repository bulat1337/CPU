#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

#include "assembler_additional.h"
#include "../global/commands.h"
#include "file_parse.h"
#include "../../stack_src/stack.h"

#define LOG_BUFFER(buf, size)\
	CPU_LOG("\nBuffer log from %s:\n", __func__);\
	print_binary(buf, size, #buf)

#define CURRENT_JMP_PTR\
	jmp_poses_w_carriage->JMP_poses[jmp_poses_w_carriage->carriage]

#define BYTE_CODE\
	result.buf_w_info

#define FILE_PTR_CHECK(file_ptr)									\
	if(file_ptr == NULL)											\
	{																\
		CPU_LOG("\nERROR: Unable to open "#file_ptr"\n");			\
		result.error_code = ASM_UNABLE_TO_OPEN_FILE;                \
		return result;							  					\
	}

struct Parse_human_code_result parse_human_code(const char *file_name)
{
	struct Parse_human_code_result result =
	{
		.error_code = ASM_ALL_GOOD,
	};

	FILE *human_code = fopen(file_name, "r");
	FILE_PTR_CHECK(human_code);

	size_t human_code_file_length = get_file_length(human_code);
	result.human_code_buffer =
	{
		.length = human_code_file_length,
		.buf = (char *)calloc(human_code_file_length, sizeof(char)),
	};

	fread(result.human_code_buffer.buf, sizeof(char), result.human_code_buffer.length, human_code);

	fclose(human_code);

	result.amount_of_lines = count_file_lines(result.human_code_buffer);
	CPU_LOG("amount of lines: %lu\n", result.amount_of_lines);

	result.strings = (char * *)calloc(result.amount_of_lines, sizeof(char *));

	ptr_arranger(result.strings, result.human_code_buffer);

	return result;
}

struct Cmds_process_result cmds_process(char * *commands, size_t amount_of_lines)
{
	struct Cmds_process_result result =
	{
		.error_code = ASM_ALL_GOOD,
	};

	size_t byte_code_size = amount_of_lines * sizeof(double) * 2; // how to optimize?
	result.buf_w_info =
	{
		.buf = (char *)calloc(byte_code_size, sizeof(char)),
		.carriage = 0,
		.length = byte_code_size,
	};

	result.labels_w_carriage =
	{
		.labels = (Label *)calloc(amount_of_lines, sizeof(Label)),
		.carriage = 0,
	};
	result.jmp_poses_w_carriage =
	{
		.JMP_poses = (JMP_pos *)calloc(amount_of_lines, sizeof(JMP_pos)),
		.carriage = 0,
	};

	write_main_jmp(&BYTE_CODE, &(result.jmp_poses_w_carriage));

	char cmd_type = (char)ZERO;
	double argument_value = NAN;
	char reg_type = 0;
	unsigned int RAM_address = 0;

	#define CURRENT_LABEL\
		result.labels_w_carriage.labels[result.labels_w_carriage.carriage]

	#define CURRENT_JMP\
		result.jmp_poses_w_carriage.JMP_poses[result.jmp_poses_w_carriage.carriage]

	#define GET_REG_TYPE(cmd)\
		*(commands[line_ID] + strlen(cmd) + 1 + 1) - 'a'

	#define IS_COMMAND(cmd)\
		!strncmp(commands[line_ID], cmd, strlen(cmd))

	size_t buf_carriage = 1;
	SAFE_FOR_START(size_t line_ID = 0; line_ID < amount_of_lines; line_ID++)
	{
		if(IS_COMMAND("push"))
		{
			cmd_type = (char)PUSH;
			write_to_buf(&BYTE_CODE, &cmd_type, sizeof(char));
			if(*(commands[line_ID] + strlen("push")) == '[')
			{
				if(sscanf(commands[line_ID] + strlen("push") + 1, "%d", &RAM_address) == 0)
				{
					align_buffer(&BYTE_CODE, 1);
					write_to_buf(&BYTE_CODE, &IDENTIFIER_BYTE, sizeof(char));
					write_to_buf(&BYTE_CODE, &IDENTIFIER_BYTE, sizeof(char));

					reg_type = GET_REG_TYPE("push");
					write_char_w_alignment(&BYTE_CODE, reg_type, ALIGN_TO_INT);
				}
				else
				{
					write_to_buf(&BYTE_CODE, &IDENTIFIER_BYTE, sizeof(char));
					align_buffer(&BYTE_CODE, 1);
					write_to_buf(&BYTE_CODE, &IDENTIFIER_BYTE, sizeof(char));

					write_to_buf(&BYTE_CODE, &RAM_address, sizeof(int));
				}
			}
			else if(sscanf(commands[line_ID] + strlen("push") ,"%lf", &argument_value) == 0)
			{
				align_buffer(&BYTE_CODE, 1);
				write_to_buf(&BYTE_CODE, &IDENTIFIER_BYTE, sizeof(char));
				align_buffer(&BYTE_CODE, 1);

				reg_type = GET_REG_TYPE("push");
				write_char_w_alignment(&BYTE_CODE, reg_type, ALIGN_TO_INT);

				buf_carriage++;
			}
			else
			{
				write_to_buf(&BYTE_CODE, &IDENTIFIER_BYTE, sizeof(char));
				align_buffer(&BYTE_CODE, 6);
				buf_carriage++;

				write_to_buf(&BYTE_CODE, &argument_value, sizeof(double));

				buf_carriage++;
			}
		}
		else if(IS_COMMAND("pop"))
		{
			cmd_type = (char)POP;
			write_to_buf(&BYTE_CODE, &cmd_type, sizeof(char));

			if(*(commands[line_ID] + strlen("pop")) == '[')
			{
				align_buffer(&BYTE_CODE, 2);
				write_to_buf(&BYTE_CODE, &IDENTIFIER_BYTE, sizeof(char));

				sscanf(commands[line_ID] + strlen("pop") + 1, "%d", &RAM_address);

				write_to_buf(&BYTE_CODE, &RAM_address, sizeof(int));
			}
			else
			{
				align_buffer(&BYTE_CODE, 1);
				write_to_buf(&BYTE_CODE, &IDENTIFIER_BYTE, sizeof(char));
				align_buffer(&BYTE_CODE, 1);

				reg_type = GET_REG_TYPE("pop");
				write_char_w_alignment(&BYTE_CODE, reg_type, ALIGN_TO_INT);
			}

			buf_carriage++;
		}
		else if(IS_COMMAND("in"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)IN, ALIGN_TO_DOUBLE);

			buf_carriage++;
		}
		else if(IS_COMMAND("add"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)ADD, ALIGN_TO_DOUBLE);

			buf_carriage++;
		}
		else if(IS_COMMAND("sub"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)SUB, ALIGN_TO_DOUBLE);

			buf_carriage++;
		}
		else if(IS_COMMAND("mul"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)MUL, ALIGN_TO_DOUBLE);

			buf_carriage++;
		}
		else if(IS_COMMAND("div"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)DIV, ALIGN_TO_DOUBLE);

			buf_carriage++;
		}
		else if(IS_COMMAND("out"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)OUT, ALIGN_TO_DOUBLE);

			buf_carriage++;
		}
		else if(IS_COMMAND("ret"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)RET, ALIGN_TO_DOUBLE);

			buf_carriage++;
		}
		else if(IS_COMMAND("jmp"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)JMP, ALIGN_TO_INT);
			write_to_buf(&BYTE_CODE, &POISON_JMP_POS, sizeof(int));

			CURRENT_JMP.name = commands[line_ID] + strlen("jmp") + 1;
			CURRENT_JMP.IP_pos = (int)buf_carriage;

			result.jmp_poses_w_carriage.carriage++;
			buf_carriage++;
		}
		else if(IS_COMMAND("jae"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)JAE, ALIGN_TO_INT);
			write_to_buf(&BYTE_CODE, &POISON_JMP_POS, sizeof(int));

			CURRENT_JMP.name = commands[line_ID] + strlen("jae") + 1;
			CURRENT_JMP.IP_pos = (int)buf_carriage;

			result.jmp_poses_w_carriage.carriage++;
			buf_carriage++;
		}
		else if(IS_COMMAND("ja"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)JA, ALIGN_TO_INT);
			write_to_buf(&BYTE_CODE, &POISON_JMP_POS, sizeof(int));

			CURRENT_JMP.name = commands[line_ID] + strlen("ja") + 1;
			CURRENT_JMP.IP_pos = (int)buf_carriage;

			result.jmp_poses_w_carriage.carriage++;
			buf_carriage++;
		}
		else if(IS_COMMAND("jbe"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)JBE, ALIGN_TO_INT);
			write_to_buf(&BYTE_CODE, &POISON_JMP_POS, sizeof(int));

			CURRENT_JMP.name   = commands[line_ID] + strlen("jbe") + 1;
			CURRENT_JMP.IP_pos = (int)buf_carriage;

			result.jmp_poses_w_carriage.carriage++;
			buf_carriage++;
		}
		else if(IS_COMMAND("jb"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)JB, ALIGN_TO_INT);
			write_to_buf(&BYTE_CODE, &POISON_JMP_POS, sizeof(int));

			CURRENT_JMP.name = commands[line_ID] + strlen("jb") + 1;
			CURRENT_JMP.IP_pos = (int)buf_carriage;

			result.jmp_poses_w_carriage.carriage++;
			buf_carriage++;
		}
		else if(IS_COMMAND("je"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)JE, ALIGN_TO_INT);
			write_to_buf(&BYTE_CODE, &POISON_JMP_POS, sizeof(int));

			CURRENT_JMP.name = commands[line_ID] + strlen("je") + 1;
			CURRENT_JMP.IP_pos = (int)buf_carriage;

			result.jmp_poses_w_carriage.carriage++;
			buf_carriage++;
		}
		else if(IS_COMMAND("jne"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)JNE, ALIGN_TO_INT);
			write_to_buf(&BYTE_CODE, &POISON_JMP_POS, sizeof(int));

			CURRENT_JMP.name = commands[line_ID] + strlen("jne") + 1;
			CURRENT_JMP.IP_pos = (int)buf_carriage;

			result.jmp_poses_w_carriage.carriage++;
			buf_carriage++;
		}
		else if(IS_COMMAND("call"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)CALL, ALIGN_TO_INT);
			write_to_buf(&BYTE_CODE, &POISON_JMP_POS, sizeof(int));

			CURRENT_JMP.name = commands[line_ID] + strlen("call") + 1;
			CURRENT_JMP.IP_pos = (int)buf_carriage;

			result.jmp_poses_w_carriage.carriage++;
			buf_carriage++;
		}
		else if(IS_COMMAND("hlt"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)HLT, ALIGN_TO_DOUBLE);
			buf_carriage++;
		}
		else if(IS_COMMAND(":"))
		{
			CURRENT_LABEL.name   = commands[line_ID] + strlen(":");
			CURRENT_LABEL.IP_pos = buf_carriage;

			result.labels_w_carriage.carriage++;
		}

		SAFE_FOR_END
	}

	LOG_BUFFER(BYTE_CODE.buf, BYTE_CODE.length);
	log_labels(&(result.labels_w_carriage));
	log_jmps(&(result.jmp_poses_w_carriage));


	return result;
}

#undef CURRENT_LABEL
#undef GET_REG_TYPE
#undef IS_COMMAND
#undef CURRENT_JMP

error_t write_main_jmp(struct Buf_w_carriage_n_len *byte_code,
					   JMP_poses_w_carriage *jmp_poses_w_carriage)
{
	write_char_w_alignment(byte_code, JMP, ALIGN_TO_INT);

	write_to_buf(byte_code, &POISON_JMP_POS, sizeof(int));

	CURRENT_JMP_PTR.name   = MAIN_JMP_NAME;
	CURRENT_JMP_PTR.IP_pos = 0;
	jmp_poses_w_carriage->carriage++;

	return ASM_ALL_GOOD;
}

error_t write_to_buf(struct Buf_w_carriage_n_len *byte_code,
					 const void *value, size_t size)
{
	error_t function_error = ASM_ALL_GOOD;
	const char *byte_of_value = (const char *)value;

	SAFE_FOR_START(size_t ID = 0; ID < size; ID++)
	{
		snprintf(byte_code->buf + byte_code->carriage,
				 sizeof(char) + 1, "%c", *(byte_of_value + ID));

		byte_code->carriage++;

		SAFE_FOR_END
	}

	return function_error;
}

error_t align_buffer(struct Buf_w_carriage_n_len *buf, size_t amount_of_bytes)
{
	char value = 0;
	SAFE_FOR_START(size_t byte_ID = 0; byte_ID < amount_of_bytes; byte_ID++)
	{
		write_to_buf(buf, &value, sizeof(char));

		SAFE_FOR_END
	}

	return ASM_ALL_GOOD;
}

error_t write_char_w_alignment(struct Buf_w_carriage_n_len *byte_code,
							   char value, size_t alignment_space)
{
	write_to_buf(byte_code, &value, sizeof(char));
	align_buffer(byte_code, alignment_space);

	return ASM_ALL_GOOD;
}

error_t log_labels(struct Labels_w_carriage *labels_w_carriage)
{
	CPU_LOG("\nLABELS\n");
	CPU_LOG("label_ID        IP_pos        name\n");
	SAFE_FOR_START(size_t label_ID = 0; label_ID < labels_w_carriage->carriage; label_ID++)
	{
		CPU_LOG("%8.lu%14.lu        %s\n",
			label_ID, labels_w_carriage->labels[label_ID].IP_pos,
			labels_w_carriage->labels[label_ID].name);

		SAFE_FOR_END
	}

	return ASM_ALL_GOOD;
}

error_t log_jmps(struct JMP_poses_w_carriage *jmp_poses_w_carriage)
{
	CPU_LOG("\nJMPS\n");
	CPU_LOG("JMP_ID        IP_pos        name\n");
	SAFE_FOR_START(size_t JMP_ID = 0; JMP_ID < jmp_poses_w_carriage->carriage; JMP_ID++)
	{
		CPU_LOG("%6.lu%14.lu        %s\n",
			JMP_ID, jmp_poses_w_carriage->JMP_poses[JMP_ID].IP_pos,
			jmp_poses_w_carriage->JMP_poses[JMP_ID].name);

		SAFE_FOR_END
	}

	return ASM_ALL_GOOD;
}

#define CURRENT_JMP\
	cmds_process_result.jmp_poses_w_carriage.JMP_poses[jmp_ID]
#define CURRENT_LABEL\
	cmds_process_result.labels_w_carriage.labels[label_ID]
#define FIXED_BYTE_CODE\
	result.second_arg.buf_w_info

return_t arrange_labels(struct Cmds_process_result cmds_process_result)
{
	return_t result =
	{
		.error_code            = ASM_ALL_GOOD,
		.second_arg.buf_w_info = cmds_process_result.buf_w_info,
	};

	const size_t amount_of_jmps   = cmds_process_result.jmp_poses_w_carriage.carriage;
	const size_t amount_of_labels = cmds_process_result.labels_w_carriage.carriage;
	bool label_found = false;

	SAFE_FOR_START(size_t jmp_ID = 0; jmp_ID < amount_of_jmps; jmp_ID++)
	{
		CPU_LOG("Working on %s...\n", CURRENT_JMP);

		SAFE_FOR_START(size_t label_ID = 0; label_ID < amount_of_labels; label_ID++)
		{
			CPU_LOG("\tIs it %s?\n", CURRENT_LABEL.name);
			if(!strncmp(CURRENT_LABEL.name, CURRENT_JMP.name, strlen(CURRENT_JMP.name)))
			{
				CPU_LOG("\t\tYes. Fixing...\n");

				label_found = true;

				FIXED_BYTE_CODE.carriage = sizeof(double) *
					(size_t)CURRENT_JMP.IP_pos + sizeof(int);


				/* why sizeof(int) - 1 ???????????????????????????????????????????
					почему то при записи без -1 затирается лишний байт
					если записать sizeof(short) тоже записывается на один айт больше,
					то есть от типа не зависит.
					при использовании этой функции ранее проблем не возникало
				*/
				write_to_buf(&FIXED_BYTE_CODE, &CURRENT_LABEL.IP_pos, sizeof(int) - 1);

				// LOG_BUFFER(FIXED_BYTE_CODE.buf, FIXED_BYTE_CODE.length);

				break;
			}
			else
			{
				CPU_LOG("\t\tNo\n");
			}

			SAFE_FOR_END
		}

		if(!label_found)
		{
			CPU_LOG("label for %s jmp doesn't exists\n", CURRENT_JMP.name);
			result.error_code = LABEL_DOESNT_EXIST;
		}

		SAFE_FOR_END
	}

	LOG_BUFFER(FIXED_BYTE_CODE.buf, FIXED_BYTE_CODE.length);

	return result;
}

#define CURRENT_CHUNK\
	*(long *)(buffer_w_info.buf + carriage)
#define REDUCED_BYTE_CODE\
	result.second_arg.buf_w_info

return_t reduce_buffer_size(struct Buf_w_carriage_n_len buffer_w_info)
{
	return_t result =
	{
		.error_code = ASM_ALL_GOOD,
	};

	bool non_zero_flag = true;
	bool zero_flag     = false;
	size_t tale   = 0;

	if(*(long *)(buffer_w_info.buf + buffer_w_info.length - sizeof(double)) != 0)
	{
		tale = buffer_w_info.length;
	}
	else
	{
		SAFE_FOR_START(size_t carriage = 0; carriage < buffer_w_info.length; carriage += sizeof(long))
		{
			if(CURRENT_CHUNK == 0)
			{
				zero_flag = true;
			}
			else
			{
				non_zero_flag = true;
				zero_flag = false;
			}

			if(non_zero_flag && zero_flag)
			{
				tale = carriage;
				non_zero_flag = false;
				zero_flag = false;
			}

			SAFE_FOR_END
		}
	}

	// + 1 incase of push 0 as a last cmd
	REDUCED_BYTE_CODE.length = tale + sizeof(double);

	REDUCED_BYTE_CODE.buf =
		(char *)realloc(buffer_w_info.buf, REDUCED_BYTE_CODE.length);

	LOG_BUFFER(REDUCED_BYTE_CODE.buf, REDUCED_BYTE_CODE.length);
	CPU_LOG("reduced length: %lu * 8 bytes\n", REDUCED_BYTE_CODE.length / 8);

	return result;
}

#undef CURRENT_JMP
#undef CURRENT_LABEL
#undef FIXED_BYTE_CODE
#undef LOG_BUFFER
#undef CURRENT_JMP_PTR
#undef BYTE_CODE
#undef FILE_PTR_CHECK
