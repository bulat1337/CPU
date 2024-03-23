#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

#include "SPU_additional.h"

/**
 * @def BYTE_CODE
 * @brief Macro representing the byte code of the Virtual Machine.
 */
#define BYTE_CODE vm.rand_access_mem.byte_code

/**
 * @def LOG_BUFFER(buf, size)
 * @brief Macro for logging the buffer content.
 * @param buf Pointer to the buffer.
 * @param size Size of the buffer.
 */
#define LOG_BUFFER(buf, size) print_binary(buf, size, #buf)

/**
 * @def UPDATE_BYTE_CODE_CARRIAGE
 * @brief Macro for updating the byte code carriage position.
 */
#define UPDATE_BYTE_CODE_CARRIAGE \
    byte_code_carriage = *(unsigned int *)(CURRENT_BYTE_CODE + sizeof(int)) * sizeof(double);

/**
 * @def MOVE_CARRIAGE
 * @brief Macro for moving the byte code carriage.
 */
#define MOVE_CARRIAGE byte_code_carriage += sizeof(double)

/**
 * @def DEF_CMD(name, num, type, ...)
 * @brief Macro for defining a command in the byte code processing switch-case statement.
 * @param name Name of the command.
 * @param num Numeric representation of the command.
 * @param type Type of the command for assembler.
 * @param ... Code block representing the action of the command.
 */
#define DEF_CMD(name, num, type, ...) \
    case num:                          \
    {                                  \
        __VA_ARGS__                    \
        break;                         \
    }

error_t process(FILE *input_file, FILE *output_file)
{
	size_t byte_code_length = get_file_length(input_file);

	struct VM vm = {};
	VM_ctor(&vm);


	BYTE_CODE = (char *)calloc(byte_code_length, sizeof(char));

	FREAD(BYTE_CODE, sizeof(char), byte_code_length, input_file);

	size_t byte_code_carriage = 0;
	char command              = (char)VOID;
	size_t reg_type           = 0;
	elem_t user_entered_value = NAN;
	elem_t value              = NAN;
	elem_t value_A            = NAN;
	elem_t value_B            = NAN;
	int cmp_result            = 666;
	unsigned int RAM_address  = 0;

	#define CURRENT_BYTE_CODE\
		(BYTE_CODE + byte_code_carriage)
	while(byte_code_carriage < byte_code_length)
	{
		command = *(CURRENT_BYTE_CODE);
		switch(command)
		{
			#include "../global/cmd_definitions.h"
			default:
			{
				printf("Unknown_command\n");
			}

		}
	}

	free(BYTE_CODE);
	VM_dtor(&vm);

	return SPU_ALL_GOOD;
}

#undef CMD_DEF

error_t VM_ctor(struct VM *vm)
{
	STACK_CTOR(&(vm->user_stack), STD_USER_STACK_SIZE);
	STACK_CTOR(&(vm->ret_stack),  STD_RET_STACK_SIZE);

	return SPU_ALL_GOOD;
}

error_t VM_dtor(struct VM *vm)
{
	stack_dtor(&(vm->user_stack));
	stack_dtor(&(vm->ret_stack));

	return SPU_ALL_GOOD;
}

#undef UPDATE_BYTE_CODE_CARRIAGE
#undef MOVE_CARRIAGE
