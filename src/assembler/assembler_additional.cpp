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
	print_binary(buf, size, #buf)

#define CURRENT_JMP_PTR\
	jmp_poses_w_carriage->JMP_poses[jmp_poses_w_carriage->carriage]

#define BYTE_CODE\
	result.buf_w_info

struct Parse_human_code_result parse_human_code(const char *file_name)
{
	struct Parse_human_code_result result =
	{
		.error_code = ASM_ALL_GOOD,
	};

	FILE *human_code = fopen(file_name, "r");

	if(human_code == NULL)
	{
		result.error_code = ASM_UNABLE_TO_OPEN_FILE;
		return result;
	}

	size_t human_code_file_length = get_file_length(human_code);
	struct Buffer_w_info human_code_buffer =
	{
		.length = human_code_file_length,
		.buf = (char *)calloc(human_code_file_length, sizeof(char)),
	};

	fread(human_code_buffer.buf, sizeof(char), human_code_buffer.length, human_code);

	fclose(human_code);

	result.amount_of_lines = count_file_lines(human_code_buffer);
	CPU_LOG("amount of lines: %lu\n", result.amount_of_lines);

	result.strings = (char * *)calloc(result.amount_of_lines, sizeof(char *));

	ptr_arranger(result.strings, human_code_buffer);

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

	struct Labels_w_carriage labels_w_carriage
	{
		.labels = (Label *)calloc(amount_of_lines, sizeof(Label)),
		.carriage = 0,
	};
	struct JMP_poses_w_carriage jmp_poses_w_carriage =
	{
		.JMP_poses = (JMP_pos *)calloc(amount_of_lines, sizeof(JMP_pos)),
		.carriage = 0,
	};

	write_main_jmp(&BYTE_CODE, &jmp_poses_w_carriage);

	char cmd_type = (char)ZERO;
	double argument_value = NAN;
	char reg_type = 0;

	#define CURRENT_LABEL\
		labels_w_carriage.labels[labels_w_carriage.carriage]

	#define CURRENT_JMP\
		jmp_poses_w_carriage.JMP_poses[jmp_poses_w_carriage.carriage]

	#define GET_REG_TYPE(cmd)\
		*(commands[line_ID] + strlen(cmd) + 1 + 1) - 'a'

	#define IS_COMMAND(cmd)\
		!strncmp(commands[line_ID], cmd, sizeof(cmd) / sizeof(char) - 1)

	size_t buf_carriage = 1;
	for(size_t line_ID = 0; line_ID < amount_of_lines; line_ID++)
	{
		if(IS_COMMAND("push"))
		{
			cmd_type = (char)PUSH;
			write_to_buf(&BYTE_CODE, &cmd_type, sizeof(char));

			if(sscanf(commands[line_ID] + strlen("push") ,"%lf", &argument_value) == 0)
			{
				CPU_LOG("reg detected\n");


				CPU_LOG("reg type: %d\n", reg_type);

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
			write_char_w_alignment(&BYTE_CODE, (char)POP, ALIGN_TO_INT);

			reg_type = GET_REG_TYPE("pop");
			write_char_w_alignment(&BYTE_CODE, reg_type, ALIGN_TO_INT);

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

			jmp_poses_w_carriage.carriage++;
			buf_carriage++;
		}
		else if(IS_COMMAND("ja"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)JA, ALIGN_TO_INT);
			write_to_buf(&BYTE_CODE, &POISON_JMP_POS, sizeof(int));

			CURRENT_JMP.name = commands[line_ID] + strlen("ja") + 1;
			CURRENT_JMP.IP_pos = (int)buf_carriage;

			jmp_poses_w_carriage.carriage++;
			buf_carriage++;
		}
		else if(IS_COMMAND("jb"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)JB, ALIGN_TO_INT);
			write_to_buf(&BYTE_CODE, &POISON_JMP_POS, sizeof(int));

			CURRENT_JMP.name = commands[line_ID] + strlen("jb") + 1;
			CURRENT_JMP.IP_pos = (int)buf_carriage;

			jmp_poses_w_carriage.carriage++;
			buf_carriage++;
		}
		else if(IS_COMMAND("jae"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)JAE, ALIGN_TO_INT);
			write_to_buf(&BYTE_CODE, &POISON_JMP_POS, sizeof(int));

			CURRENT_JMP.name = commands[line_ID] + strlen("jae") + 1;
			CURRENT_JMP.IP_pos = (int)buf_carriage;

			jmp_poses_w_carriage.carriage++;
			buf_carriage++;
		}
		else if(IS_COMMAND("jbe"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)JBE, ALIGN_TO_INT);
			write_to_buf(&BYTE_CODE, &POISON_JMP_POS, sizeof(int));

			CURRENT_JMP.name   = commands[line_ID] + strlen("jbe") + 1;
			CURRENT_JMP.IP_pos = (int)buf_carriage;

			jmp_poses_w_carriage.carriage++;
			buf_carriage++;
		}
		else if(IS_COMMAND("je"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)JE, ALIGN_TO_INT);
			write_to_buf(&BYTE_CODE, &POISON_JMP_POS, sizeof(int));

			CURRENT_JMP.name = commands[line_ID] + strlen("je") + 1;
			CURRENT_JMP.IP_pos = (int)buf_carriage;

			jmp_poses_w_carriage.carriage++;
			buf_carriage++;
		}
		else if(IS_COMMAND("jne"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)JNE, ALIGN_TO_INT);
			write_to_buf(&BYTE_CODE, &POISON_JMP_POS, sizeof(int));

			CURRENT_JMP.name = commands[line_ID] + strlen("jne") + 1;
			CURRENT_JMP.IP_pos = (int)buf_carriage;

			jmp_poses_w_carriage.carriage++;
			buf_carriage++;
		}
		else if(IS_COMMAND("call"))
		{
			write_char_w_alignment(&BYTE_CODE, (char)CALL, ALIGN_TO_INT);
			write_to_buf(&BYTE_CODE, &POISON_JMP_POS, sizeof(int));

			CURRENT_JMP.name = commands[line_ID] + strlen("call") + 1;
			CURRENT_JMP.IP_pos = (int)buf_carriage;

			jmp_poses_w_carriage.carriage++;
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
			CURRENT_LABEL.IP_pos = buf_carriage + 1;

			labels_w_carriage.carriage++;
		}
	}

	result.jmp_poses_w_carriage = jmp_poses_w_carriage;
	result.labels_w_carriage = labels_w_carriage;

	LOG_BUFFER(BYTE_CODE.buf, BYTE_CODE.length);
	log_labels(&labels_w_carriage);
	log_jmps(&jmp_poses_w_carriage);


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

	CURRENT_JMP_PTR.name   = "main";
	CURRENT_JMP_PTR.IP_pos = 0;
	jmp_poses_w_carriage->carriage++;

	LOG_BUFFER(byte_code->buf, byte_code->length);

	return ASM_ALL_GOOD;
}

error_t write_to_buf(struct Buf_w_carriage_n_len *byte_code,
					 const void *value, size_t size)
{
	error_t function_error = ASM_ALL_GOOD;
	const char *byte_of_value = (const char *)value;

	for(size_t ID = 0; ID < size; ID++)
	{
		snprintf(byte_code->buf + byte_code->carriage,
				 sizeof(char) + 1, "%c", *(byte_of_value + ID));

		byte_code->carriage++;
	}

	return function_error;
}

void print_binary(char *buf, size_t size, const char *buf_name)
{
	CPU_LOG("%s:\n", buf_name);
    for (size_t ID = 0; ID < size; ID++)
	{
		if(ID % 8 == 0)
		{
			CPU_LOG("double[%lu] - %lf\n", ID / 8, *(double *)(buf + ID));
		}

        char cur_elem = buf[ID];
		CPU_LOG("[%2.lu] - ", ID);

        for (int bit_offset = 7; bit_offset >= 0; bit_offset--)
		{
            CPU_LOG("%d", (cur_elem >> bit_offset) & 1);
        }

        CPU_LOG("\n");
    }
}

error_t align_buffer(struct Buf_w_carriage_n_len *buf, size_t amount_of_bytes)
{
	char value = 0;
	for(size_t byte_ID = 0; byte_ID < amount_of_bytes; byte_ID++)
	{
		write_to_buf(buf, &value, sizeof(char));
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
	for(size_t label_ID = 0; label_ID < labels_w_carriage->carriage; label_ID++)
	{
		CPU_LOG("%8.lu%14.lu        %s\n",
			label_ID, labels_w_carriage->labels[label_ID].IP_pos,
			labels_w_carriage->labels[label_ID].name);
	}

	return ASM_ALL_GOOD;
}

error_t log_jmps(struct JMP_poses_w_carriage *jmp_poses_w_carriage)
{
	CPU_LOG("\nJMPS\n");
	CPU_LOG("JMP_ID        IP_pos        name\n");
	for(size_t JMP_ID = 0; JMP_ID < jmp_poses_w_carriage->carriage; JMP_ID++)
	{
		CPU_LOG("%6.lu%14.lu        %s\n",
			JMP_ID, jmp_poses_w_carriage->JMP_poses[JMP_ID].IP_pos,
			jmp_poses_w_carriage->JMP_poses[JMP_ID].name);
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

	for(size_t jmp_ID = 0; jmp_ID < amount_of_jmps; jmp_ID++)
	{
		CPU_LOG("Working on %s...\n", CURRENT_JMP);

		for(size_t label_ID = 0; label_ID < amount_of_labels; label_ID++)
		{
			CPU_LOG("\tIs it %s?\n", CURRENT_LABEL.name);
			if(!strncmp(CURRENT_LABEL.name, CURRENT_JMP.name, strlen(CURRENT_JMP.name)))
			{
				CPU_LOG("\t\tYes. Fixing...\n");

				label_found = true;

				FIXED_BYTE_CODE.carriage = sizeof(double) *
					CURRENT_JMP.IP_pos + sizeof(int);

				write_to_buf(&FIXED_BYTE_CODE, &CURRENT_LABEL.IP_pos, sizeof(int));

				break;
			}
			else
			{
				CPU_LOG("\t\tNo\n");
			}
		}

		if(!label_found)
		{
			CPU_LOG("label for %s jmp doesn't exists\n", CURRENT_JMP.name);
			result.error_code = LABEL_DOESNT_EXIST;
		}

	}

	LOG_BUFFER(FIXED_BYTE_CODE.buf, FIXED_BYTE_CODE.length);

	return result;
}

#undef CURRENT_JMP
#undef CURRENT_LABEL
#undef FIXED_BYTE_CODE
#undef LOG_BUFFER
#undef CURRENT_JMP_PTR
#undef BYTE_CODE
