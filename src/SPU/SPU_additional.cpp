#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

#include "SPU_additional.h"

#define BYTE_CODE\
	vm.rand_access_mem.byte_code

#define LOG_BUFFER(buf, size)\
	print_binary(buf, size, #buf)

error_t process(const char *file_name)
{
	FILE *byte_code_file = fopen(file_name, "rb");

	if(byte_code_file == NULL)
	{
		CPU_LOG("Unable to open %s\n", file_name);

		return SPU_UNABLE_TO_OPEN_FILE;
	}

	size_t byte_code_length = get_file_length(byte_code_file);



	struct VM vm = {};
	VM_ctor(&vm);


	BYTE_CODE = (char *)calloc(byte_code_length, sizeof(char));

	size_t read_elems = fread(BYTE_CODE, sizeof(char), byte_code_length, byte_code_file);
	if(read_elems != byte_code_length)
	{
		CPU_LOG("ERROR: read_elems != byte_code_length\n");

		return INVALID_FREAD;
	}

	LOG_BUFFER(BYTE_CODE, byte_code_length);

	size_t byte_code_carriage = 0;
	char command              = (char)ZERO;
	int reg_type              = 0;
	double user_entered_value = NAN;
	double value              = NAN;
	double value_A            = NAN;
	double value_B            = NAN;

	#define CURRENT_BYTE_CODE\
		(BYTE_CODE + byte_code_carriage)
	while(byte_code_carriage < byte_code_length)
	{
		command = *(CURRENT_BYTE_CODE);
		// printf("command code: %d\n", commaxnd);
		switch(command)
		{
			case ZERO:
			{
				byte_code_carriage += sizeof(double);

				break;
			}
			case PUSH:
			{
				if(*(char *)(CURRENT_BYTE_CODE + sizeof(char)) != 0) //сделать проверку маской
				{
					byte_code_carriage += sizeof(double);

					STACK_PUSH(&(vm.user_stack), *(double *)CURRENT_BYTE_CODE);
				}
				//сделать проверку маской
				else if(*(CURRENT_BYTE_CODE + 2 * sizeof(char)) != 0)
				{
					reg_type = (size_t)*(int *)(CURRENT_BYTE_CODE + sizeof(int)); // GET_REG_TYPE
					STACK_PUSH(&(vm.user_stack), vm.registers[reg_type]);
				}

				byte_code_carriage += sizeof(double);

				break;
			}
			case POP:
			{
				reg_type = (size_t)*(int *)(CURRENT_BYTE_CODE + sizeof(int));
				vm.registers[reg_type] = 					//CURRENT_REGISTER
					STACK_POP(&(vm.user_stack)).deleted_element;

				byte_code_carriage += sizeof(double);

				break;
			}
			case IN:
			{
				printf("Please enter value: ");
				scanf(">\t%lf", &user_entered_value);

				STACK_PUSH(&(vm.user_stack), user_entered_value);

				byte_code_carriage += sizeof(double);

				break;
			}
			case ADD:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				STACK_PUSH(&(vm.user_stack), value_A + value_B);

				byte_code_carriage += sizeof(double);

				break;
			}
			case SUB:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				STACK_PUSH(&(vm.user_stack), value_A - value_B);

				byte_code_carriage += sizeof(double);

				break;
			}
			case MUL:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				STACK_PUSH(&(vm.user_stack), value_A * value_B);

				byte_code_carriage += sizeof(double);

				break;
			}
			case DIV:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				STACK_PUSH(&(vm.user_stack), value_A / value_B);

				byte_code_carriage += sizeof(double);

				break;
			}
			case OUT:
			{
				value = STACK_POP(&(vm.user_stack)).deleted_element;

				printf("RESULT: %lf\n", value);

				byte_code_carriage += sizeof(double);

				break;
			}
			case RET:
			{
				byte_code_carriage = STACK_POP(&(vm.ret_stack)).deleted_element;

				break;
			}
			case JMP:
			{
				byte_code_carriage =
					(size_t)*(int *)(CURRENT_BYTE_CODE + sizeof(int)) * sizeof(double);

				break;
			}
			case JA:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				if(value_A > value_B)
				{
					byte_code_carriage =
						(size_t)*(int *)(CURRENT_BYTE_CODE + sizeof(int)) * sizeof(double);
				}
				else
				{
					byte_code_carriage += sizeof(double);
				}

				break;
			}
			case JB:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				if(value_A < value_B)
				{
					byte_code_carriage =
						(size_t)*(int *)(CURRENT_BYTE_CODE + sizeof(int)) * sizeof(double);
				}
				else
				{
					byte_code_carriage += sizeof(double);
				}

				break;
			}
			case JAE:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				if(value_A >= value_B)
				{
					byte_code_carriage =
						(size_t)*(int *)(CURRENT_BYTE_CODE + sizeof(int)) * sizeof(double);
				}
				else
				{
					byte_code_carriage += sizeof(double);
				}

				break;
			}
			case JBE:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				if(value_A <= value_B)
				{
					byte_code_carriage =
						(size_t)*(int *)(CURRENT_BYTE_CODE + sizeof(int)) * sizeof(double);
				}
				else
				{
					byte_code_carriage += sizeof(double);
				}

				break;
			}
			case JE:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				if(value_A == value_B)
				{
					byte_code_carriage =
						(size_t)*(int *)(CURRENT_BYTE_CODE + sizeof(int)) * sizeof(double);
				}
				else
				{
					byte_code_carriage += sizeof(double);
				}

				break;
			}
			case JNE:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				if(value_A != value_B)
				{
					byte_code_carriage =
						(size_t)*(int *)(CURRENT_BYTE_CODE + sizeof(int)) * sizeof(double);
				}
				else
				{
					byte_code_carriage += sizeof(double);
				}

				break;
			}
			case CALL:
			{
				STACK_PUSH(&(vm.ret_stack), byte_code_carriage + sizeof(double));

				byte_code_carriage =
					(size_t)*(int *)(CURRENT_BYTE_CODE + sizeof(int)) * sizeof(double);

				break;
			}
			case HLT:
			{
				return SPU_ALL_GOOD;
			}
			deafult:
			{
				printf("Unknown_command\n");
			}

		}
	}


	return SPU_ALL_GOOD;
}

error_t VM_ctor(struct VM *vm)
{
	STACK_CTOR(&(vm->user_stack), 10);
	STACK_CTOR(&(vm->ret_stack), 2);

	return SPU_ALL_GOOD;
}
