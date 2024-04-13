#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

#include "assembler_additional.h"
#include "commands.h"
#include "file_parse.h"
#include "stack.h"

#define ALLOCATION_CHECK(ptr)\
	if(ptr == NULL)\
	{\
		CPU_LOG("Unable to allocate"#ptr".\n");	\
		return ASM_UNABLE_TO_ALLOCATE;			\
	}

#define CALLOC(ptr, amount, type)				\
	ptr = (type *)calloc(amount, sizeof(type));	\
	ALLOCATION_CHECK(ptr);

#define REALLOC(ptr, amount, type)					\
	ptr = (type *)realloc(ptr, amount);				\
	if(ptr == NULL)									\
	{												\
		CPU_LOG("Unable to reallocate"#ptr".\n");	\
		return ASM_UNABLE_TO_ALLOCATE;				\
	}

/**
 * @def LOG_BUFFER
 * @brief Macro to log buffer contents.
 *
 * This macro logs the contents of a buffer using the CPU_LOG macro. It prints the buffer
 * contents along with the function name where the log is called.
 *
 * @param buf Pointer to the buffer.
 * @param size Size of the buffer.
 */
#define LOG_BUFFER(buf, size)\
    CPU_LOG("\nBuffer log from %s:\n", __func__);\
    print_binary(buf, size, #buf)

/**
 * @def CURRENT_JMP_PTR
 * @brief Macro to get the current jump pointer.
 *
 * This macro retrieves the current jump pointer from the 'jmp_poses_w_carriage' structure.
 */
#define CURRENT_JMP_PTR\
    jmp_poses_w_carriage->JMP_poses[jmp_poses_w_carriage->carriage]

/**
 * @def BYTE_CODE
 * @brief Macro representing the byte code buffer.
 *
 */
#define BYTE_CODE\
    manager->byte_code

/**
 * @def FILE_PTR_CHECK
 * @brief Macro to check if a file pointer is valid.
 *
 * @param file_ptr Pointer to the file.
 */
#define FILE_PTR_CHECK(file_ptr)                                    \
    if(file_ptr == NULL)                                            \
    {                                                               \
        CPU_LOG("\nERROR: Unable to open "#file_ptr"\n");           \
        return ASM_UNABLE_TO_OPEN_FILE;								\
    }

error_t parse_human_code(Compile_manager *manager, const char *file_name)
{
	WITH_OPEN
	(
		file_name, "r", human_code,

		size_t human_code_file_length = get_file_length(human_code);
		manager->human_code_buffer =
		{
			.length = human_code_file_length,
		};
		CALLOC(manager->human_code_buffer.buf, human_code_file_length, char);

		FREAD(manager->human_code_buffer.buf, sizeof(char),
		      manager->human_code_buffer.length, human_code);
	)

	manager->strings.amount = count_file_lines(manager->human_code_buffer);
	CPU_LOG("amount of lines: %lu\n", manager->strings.amount);

	CALLOC(manager->strings.tokens, manager->strings.amount, char *);

	ptr_arranger(manager->strings.tokens, manager->human_code_buffer); //rename

	return ASM_ALL_GOOD;
}

#define COMMANDS\
	manager->strings.tokens

#define WRITE_BYTE(ptr)\
	write_to_buf(&BYTE_CODE, ptr, sizeof(char));

#define WRITE_INT(ptr)\
	write_to_buf(&BYTE_CODE, ptr, sizeof(int));

#define ALIGN_BUF(amount)\
	align_buffer(&BYTE_CODE, amount);




#define PROCESS_RAM_ARG(cmd_name)\
 	if(*(cmd_arg) == '[')												\
 	{/* RAM */															\
 		if(sscanf(cmd_arg, "[%d]", &RAM_address) == 0)					\
 		{/* RAM w REG */												\
 			mask_buffer(&(manager->byte_code), RAM_MASK | REG_MASK);	\
 			ALIGN_BUF(TWO_BYTE_ALIGNMENT);								\
 																		\
 			reg_type = GET_REG_TYPE(cmd_name);							\
 			write_char_w_alignment(&BYTE_CODE, reg_type, ALIGN_TO_INT);	\
 		}																\
 		else															\
 		{/* RAM w IMM*/													\
 			mask_buffer(&(manager->byte_code), RAM_MASK | IMM_MASK);	\
 			ALIGN_BUF(TWO_BYTE_ALIGNMENT);								\
 																		\
 			WRITE_INT(&RAM_address);									\
 		}																\
 	}

/**
 * @def WRITE_CMD_W_8_BYTE_ARG
 * @brief Macro to write a command with an 8-byte argument to the byte code buffer.
 *
 * This macro writes a command with an 8-byte argument to the byte code buffer.
 * Depending on the command type, it writes the argument, and any additional
 * information to the byte code buffer.
 *
 * @param cmd_name The name of the command.
 * @param num The numerical representation of the command.
 */
#define WRITE_CMD_W_8_BYTE_ARG(cmd_name, num)								\
	else if(IS_COMMAND(cmd_name))											\
	{																		\
		cmd_type = (Command)num;											\
		WRITE_BYTE(&cmd_type);												\
		char *cmd_arg = COMMANDS[line_ID] + LEN(cmd_name);					\
																			\
		PROCESS_RAM_ARG(cmd_name)											\
		else if(sscanf(cmd_arg, "%lf", &argument_value) == 0)				\
		{/* REG */															\
			mask_buffer(&(manager->byte_code), REG_MASK);					\
			ALIGN_BUF(TWO_BYTE_ALIGNMENT);									\
																			\
			reg_type = GET_REG_TYPE(cmd_name);								\
			write_char_w_alignment(&BYTE_CODE, reg_type, ALIGN_TO_INT);		\
																			\
		}																	\
		else																\
		{/* IMM */															\
			mask_buffer(&(manager->byte_code), IMM_MASK);					\
			ALIGN_BUF(SIX_BYTE_ALIGNMENT);									\
																			\
			write_to_buf(&BYTE_CODE, &argument_value, sizeof(elem_t));		\
																			\
		}																	\
	}

/**
 * @def WRITE_CMD_W_4_BYTE_ARG
 * @brief Macro to write a command with a 4-byte argument to the byte code buffer.
 *
 * This macro writes a command with a 4-byte argument to the byte code buffer. Depending
 * on the command type, it writes and any additional information to the byte code buffer.
 *
 * @param cmd_name The name of the command.
 * @param num The numerical representation of the command.
 */
#define WRITE_CMD_W_4_BYTE_ARG(cmd_name, num)								\
	else if(IS_COMMAND(cmd_name))											\
	{																		\
		cmd_type = (Command)num;											\
		WRITE_BYTE(&cmd_type);												\
		char *cmd_arg = COMMANDS[line_ID] + LEN(cmd_name);					\
																			\
		PROCESS_RAM_ARG(cmd_name)											\
		else																\
		{																	\
			mask_buffer(&(manager->byte_code), REG_MASK);					\
			ALIGN_BUF(TWO_BYTE_ALIGNMENT);									\
																			\
			reg_type = GET_REG_TYPE(cmd_name);								\
			write_char_w_alignment(&BYTE_CODE, reg_type, ALIGN_TO_INT);		\
		}																	\
	}

/**
 * @def WRITE_CMD_W_NO_ARG
 * @brief Macro to write a command with no argument to the byte code buffer.
 *
 * This macro writes a command with no argument to the byte code buffer. Depending
 * on the command type, it writes the command type to the byte code buffer.
 *
 * @param cmd_name The name of the command.
 * @param num The numerical representation of the command.
 */
#define WRITE_CMD_W_NO_ARG(cmd_name, num)									\
	else if(IS_COMMAND(cmd_name))											\
	{																		\
		write_char_w_alignment(&BYTE_CODE, (char)num, ALIGN_TO_DOUBLE);		\
	}

/**
 * @def WRITE_CMD_W_LABEL_ARG
 * @brief Macro to write a command with a label argument to the byte code buffer.
 *
 * This macro writes a command with a label argument to the byte code buffer. Depending
 * on the command type, it writes the command type, the label argument, and any additional
 * information to the byte code buffer.
 *
 * @param cmd_name The name of the command.
 * @param num The numerical representation of the command.
 */
#define WRITE_CMD_W_LABEL_ARG(cmd_name, num)								\
	else if(IS_COMMAND(cmd_name))											\
	{																		\
		write_char_w_alignment(&BYTE_CODE, (char)num, ALIGN_TO_INT);		\
		WRITE_INT(&POISON_JMP_POS);											\
																			\
		CURRENT_JMP.name = COMMANDS[line_ID] + LEN(cmd_name) + 1;			\
		CURRENT_JMP.IP_pos = (int)get_ip_pos(manager) - 1;					\
																			\
		manager->jmp_poses_w_carriage.carriage++;							\
	}

/**
 * @def WRITE_LABEL
 * @brief Macro to write a label to the byte code buffer.
 *
 * This macro writes a label to the byte code buffer. It then writes
 * the label name and its position to the byte code buffer.
 *
 * @param cmd_name The name of the command.
 * @param num The numerical representation of the command.
 */
#define WRITE_LABEL(cmd_name, num)													\
	else if(IS_COMMAND(cmd_name))													\
	{																				\
		CURRENT_LABEL.name   = COMMANDS[line_ID] + LEN(":");						\
		CURRENT_LABEL.IP_pos = get_ip_pos(manager);									\
																					\
		manager->labels_w_carriage.carriage++;										\
	}

#define WRITE_CMD_W_2_ARGS(cmd_name, num)									\
	else if(IS_COMMAND(cmd_name))											\
	{																		\
		cmd_type = (Command)num;											\
		write_char_w_alignment(&BYTE_CODE, (char)num, ALIGN_TO_DOUBLE);		\
																			\
		char *cmd_arg = COMMANDS[line_ID] + LEN(cmd_name);					\
																			\
		unsigned int head  = 0;												\
		unsigned int end   = 0;												\
																			\
		sscanf(cmd_arg, "%u %u", &head, &end);								\
																			\
		write_to_buf(&BYTE_CODE, &head, sizeof(int));						\
		write_to_buf(&BYTE_CODE, &end, sizeof(int));						\
																			\
	}

/**
 * @def DEF_CMD
 * @brief Macro to define a command with different argument types.
 *
 * @param name The name of the command.
 * @param num The numerical representation of the command.
 * @param type The type of the command (e.g., WRITE_CMD_W_8_BYTE_ARG).
 * @param ... Additional arguments.
 */
#define DEF_CMD(name, num, type, ...)	\
	type(name, num)


error_t cmds_process(Compile_manager *manager)
{
	size_t amount_of_lines = manager->strings.amount;

	size_t byte_code_size = amount_of_lines * sizeof(double) * 2;

	CALLOC(manager->byte_code.buf, byte_code_size, char);
	manager->byte_code_start = manager->byte_code.buf;

	manager->byte_code.length = byte_code_size;

	manager->labels_w_carriage =
	{
		.carriage = 0,
	};
	CALLOC(manager->labels_w_carriage.labels, amount_of_lines, Label)

	manager->jmp_poses_w_carriage =
	{
		.carriage = 0,
	};
	CALLOC(manager->jmp_poses_w_carriage.JMP_poses, amount_of_lines, JMP_pos);

	write_main_jmp(&BYTE_CODE, &(manager->jmp_poses_w_carriage));

	char cmd_type = (char)VOID;
	elem_t argument_value = NAN;
	char reg_type = 0;
	unsigned int RAM_address = 0;

	#define CURRENT_LABEL\
		manager->labels_w_carriage.labels[manager->labels_w_carriage.carriage]

	#define CURRENT_JMP\
		manager->jmp_poses_w_carriage.JMP_poses[manager->jmp_poses_w_carriage.carriage]

	#define GET_REG_TYPE(cmd)\
		*(COMMANDS[line_ID] + LEN(cmd) + SPACE_SKIP + LETTER_SKIP) - 'a'

	#define IS_COMMAND(cmd)\
		!strncmp(COMMANDS[line_ID], cmd, LEN(cmd))

	SAFE_FOR_START(size_t line_ID = 0; line_ID < amount_of_lines; line_ID++)
	{
		if(false)
		{
			;
		}
		#include "cmd_definitions.h"

		SAFE_FOR_END
	}

	LOG_BUFFER(manager->byte_code_start, BYTE_CODE.length);
	log_labels(&(manager->labels_w_carriage));
	log_jmps(&(manager->jmp_poses_w_carriage));

	manager->byte_code.buf = manager->byte_code_start;

	return ASM_ALL_GOOD;
}

#undef WRITE_CMD_W_4_BYTE_ARG
#undef WRITE_CMD_W_8_BYTE_ARG
#undef WRITE_CMD_W_LABEL_ARG
#undef WRITE_CMD_W_NO_ARG
#undef WRITE_LABEL
#undef DEF_CMD
#undef CURRENT_LABEL
#undef GET_REG_TYPE
#undef IS_COMMAND
#undef CURRENT_JMP

error_t write_main_jmp(struct Buffer_w_info *byte_code,
					   JMP_poses_w_carriage *jmp_poses_w_carriage)
{
	write_char_w_alignment(byte_code, JMP, ALIGN_TO_INT);

	write_to_buf(byte_code, &POISON_JMP_POS, sizeof(int));

	CURRENT_JMP_PTR.name   = MAIN_JMP_NAME;
	CURRENT_JMP_PTR.IP_pos = 0;
	jmp_poses_w_carriage->carriage++;

	return ASM_ALL_GOOD;
}

error_t write_to_buf(struct Buffer_w_info *byte_code,
					 const void *value, size_t size)
{
	error_t function_error = ASM_ALL_GOOD;
	const char *byte_of_value = (const char *)value;

	SAFE_FOR_START(size_t ID = 0; ID < size; ID++)
	{
		snprintf(byte_code->buf, sizeof(char) + 1, "%c", *(byte_of_value + ID));

		byte_code->buf++;

		SAFE_FOR_END
	}

	return function_error;
}

error_t align_buffer(struct Buffer_w_info *buf, size_t amount_of_bytes)
{
	char value = 0;
	SAFE_FOR_START(size_t byte_ID = 0; byte_ID < amount_of_bytes; byte_ID++)
	{
		write_to_buf(buf, &value, sizeof(char));

		SAFE_FOR_END
	}

	return ASM_ALL_GOOD;
}

error_t write_char_w_alignment(struct Buffer_w_info *byte_code,
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
	manager->jmp_poses_w_carriage.JMP_poses[jmp_ID]
#define CURRENT_LABEL\
	manager->labels_w_carriage.labels[label_ID]

error_t arrange_labels(Compile_manager *manager)
{
	error_t error_code = ASM_ALL_GOOD;

	const size_t amount_of_jmps   = manager->jmp_poses_w_carriage.carriage;
	const size_t amount_of_labels = manager->labels_w_carriage.carriage;

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

				BYTE_CODE.buf += sizeof(double) *
					(size_t)CURRENT_JMP.IP_pos + sizeof(int);


				/* why sizeof(int) - 1 ???????????????????????????????????????????
					почему то при записи без -1 затирается лишний байт
					если записать sizeof(short) тоже записывается на один айт больше,
					то есть от типа не зависит.
					при использовании этой функции ранее проблем не возникало
				*/
				write_to_buf(&BYTE_CODE, &CURRENT_LABEL.IP_pos, sizeof(int) - 1);

				BYTE_CODE.buf = manager->byte_code_start;

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
			error_code = LABEL_DOESNT_EXIST;
		}

		SAFE_FOR_END
	}

	LOG_BUFFER(manager->byte_code_start, BYTE_CODE.length);

	CPU_LOG("Byte_code size: %lu * sizeof(double) bytes\n",
			 manager->byte_code.length / sizeof(double));

	manager->byte_code.buf = manager->byte_code_start;

	return error_code;
}

#define CURRENT_CHUNK\
	*(long *)(manager->byte_code.buf + carriage)

error_t reduce_buffer_size(Compile_manager *manager)
{
	bool non_zero_flag = true;
	bool zero_flag     = false;
	size_t tale        = 0;

	if(*(long *)(BYTE_CODE.buf + BYTE_CODE.length - sizeof(double)) != 0)
	{
		tale = BYTE_CODE.length;
	}
	else
	{
		SAFE_FOR_START(size_t carriage = 0; carriage < BYTE_CODE.length; carriage += sizeof(long))
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
	BYTE_CODE.length = tale + sizeof(double);

	REALLOC(BYTE_CODE.buf, BYTE_CODE.length, char);
	manager->byte_code_start = BYTE_CODE.buf;

	LOG_BUFFER(BYTE_CODE.buf, BYTE_CODE.length);
	CPU_LOG("reduced length: %lu * sizeof(double) bytes\n",
			BYTE_CODE.length / sizeof(double));

	return ASM_ALL_GOOD;
}

char *create_file_name(const char *name, const char *postfix)
{
	size_t byte_code_file_name_size =
		strlen(postfix) + strlen(name) + ADDITIONAL_CONCATENATION_SPACE;

	char *byte_code_file_name =
		(char *)calloc(byte_code_file_name_size, sizeof(char));
	if(byte_code_file_name == NULL)
	{
		return NULL;
	}

	snprintf(byte_code_file_name, byte_code_file_name_size, "%s%s", name, postfix);

	return byte_code_file_name;
}

error_t create_bin(Compile_manager *manager, const char *file_name)
{
	char *byte_code_file_name = create_file_name(file_name, ".bin");
	ALLOCATION_CHECK(byte_code_file_name);

	WITH_OPEN
	(
		byte_code_file_name, "wb", byte_code,

		free(byte_code_file_name);

		FWRITE(manager->byte_code.buf, sizeof(char),
			   manager->byte_code.length, byte_code);
	)

	return ASM_ALL_GOOD;
}

error_t manager_dtor(Compile_manager *manager)
{
	free(manager->byte_code.buf);
	free(manager->human_code_buffer.buf);
	free(manager->jmp_poses_w_carriage.JMP_poses);
	free(manager->labels_w_carriage.labels);
	free(manager->strings.tokens);

	manager->byte_code.buf                  = NULL;
 	manager->human_code_buffer.buf          = NULL;
 	manager->jmp_poses_w_carriage.JMP_poses = NULL;
 	manager->labels_w_carriage.labels       = NULL;
 	manager->strings.tokens                 = NULL;

	manager->byte_code.length 				= 0;
	manager->human_code_buffer.length 		= 0;
	manager->jmp_poses_w_carriage.carriage 	= 0;
	manager->labels_w_carriage.carriage 	= 0;
	manager->strings.amount 				= 0;

	return ASM_ALL_GOOD;
}

error_t mask_buffer(Buffer_w_info *byte_code, const char mask)
{
	*(byte_code->buf) |= mask;

	byte_code->buf++;

	return ASM_ALL_GOOD;
}

error_t init_manager(Compile_manager *manager)
{
	manager->byte_code.buf                  = NULL;
	manager->byte_code.length               = 0;

	manager->human_code_buffer.buf          = NULL;
	manager->human_code_buffer.length       = 0;

	manager->jmp_poses_w_carriage.JMP_poses = NULL;
	manager->jmp_poses_w_carriage.carriage  = 0;

	manager->labels_w_carriage.labels       = NULL;
	manager->labels_w_carriage.carriage     = 0;

	manager->strings.tokens                 = NULL;
	manager->strings.amount                 = 0;

	manager->byte_code_start                = NULL;

	return ASM_ALL_GOOD;
}

size_t get_ip_pos(Compile_manager *manager)
{
	return (size_t)(BYTE_CODE.buf - manager->byte_code_start) / sizeof(double);
}

#undef CURRENT_JMP
#undef CURRENT_LABEL
#undef FIXED_BYTE_CODE
#undef LOG_BUFFER
#undef CURRENT_JMP_PTR
#undef BYTE_CODE
#undef FILE_PTR_CHECK
#undef ALLOCATION_CHECK
#undef CALLOC
#undef REALLOC
