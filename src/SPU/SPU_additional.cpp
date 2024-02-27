#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

#include "SPU_additional.h"

#define BYTE_CODE\
	vm.rand_access_mem.byte_code

#define LOG_BUFFER(buf, size)\
	print_binary(buf, size, #buf)

#define UPDATE_BYTE_CODE_CARRIAGE											\
	byte_code_carriage =													\
		*(unsigned int *)(CURRENT_BYTE_CODE + sizeof(int)) * sizeof(double);

#define MOVE_CARRIAGE\
	byte_code_carriage += sizeof(double)

#define DEF_CMD(name, num, type, ...)\
	case num:\
	{\
		__VA_ARGS__\
		break;\
	}

error_t process(FILE *input_file, FILE *output_file)
{
	size_t byte_code_length = get_file_length(input_file);


	struct VM vm = {};
	VM_ctor(&vm);


	BYTE_CODE = (char *)calloc(byte_code_length, sizeof(char));

	size_t read_elems = fread(BYTE_CODE, sizeof(char), byte_code_length, input_file);

	if(read_elems != byte_code_length)
	{
		CPU_LOG("ERROR: read_elems != byte_code_length\n");

		return INVALID_FREAD;
	}

	size_t byte_code_carriage = 0;
	char command              = (char)ZERO;
	size_t reg_type           = 0;
	double user_entered_value = NAN;
	double value              = NAN;
	double value_A            = NAN;
	double value_B            = NAN;
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
	STACK_CTOR(&(vm->user_stack), 10);
	STACK_CTOR(&(vm->ret_stack), 2);

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
