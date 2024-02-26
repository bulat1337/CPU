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
	fclose(byte_code_file);

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
			case ZERO:
			{
				MOVE_CARRIAGE;

				break;
			}
			case PUSH:
			{
				if(*(int *)(CURRENT_BYTE_CODE) & RAM_IDENTIFIER_MASK)
				{
					if(*(int *)(CURRENT_BYTE_CODE) & CONSTANT_IDENTIFIER_MASK)
					{
						RAM_address = *(unsigned int *)(CURRENT_BYTE_CODE + sizeof(int));

						value = vm.rand_access_mem.user_RAM[RAM_address];

						STACK_PUSH(&(vm.user_stack), value);
					}
					else if(*(int *)(CURRENT_BYTE_CODE) & REGISTER_IDENTIFIER_MASK)
					{
						reg_type = *(unsigned int *)(CURRENT_BYTE_CODE + sizeof(int));

						RAM_address = (unsigned int)vm.registers[reg_type];

						value = vm.rand_access_mem.user_RAM[RAM_address];

						STACK_PUSH(&(vm.user_stack), value);
					}
					else
					{
						return INVALID_RAM_MODE;
					}
				}
				else if(*(int *)(CURRENT_BYTE_CODE) & CONSTANT_IDENTIFIER_MASK)
				{
					MOVE_CARRIAGE;

					STACK_PUSH(&(vm.user_stack), *(double *)CURRENT_BYTE_CODE);
				}
				else if(*(int *)(CURRENT_BYTE_CODE) & REGISTER_IDENTIFIER_MASK)
				{
					reg_type = *(unsigned int *)(CURRENT_BYTE_CODE + sizeof(int));
					STACK_PUSH(&(vm.user_stack), vm.registers[reg_type]);
				}

				MOVE_CARRIAGE;

				break;
			}
			case POP:
			{
				if(*(int *)(CURRENT_BYTE_CODE) & RAM_IDENTIFIER_MASK)
				{
					RAM_address = *(unsigned int *)(CURRENT_BYTE_CODE + sizeof(int));

					vm.rand_access_mem.user_RAM[RAM_address] =
						STACK_POP(&(vm.user_stack)).deleted_element;
				}
				else if(*(int *)(CURRENT_BYTE_CODE) & REGISTER_IDENTIFIER_MASK)
				{
					reg_type = *(unsigned int *)(CURRENT_BYTE_CODE + sizeof(int));

					vm.registers[reg_type] =
						STACK_POP(&(vm.user_stack)).deleted_element;
				}


				MOVE_CARRIAGE;

				break;
			}
			case IN:
			{
				printf("Please enter value: ");

				scanf("%lf", &user_entered_value);
				clear_buffer();

				STACK_PUSH(&(vm.user_stack), user_entered_value);

				MOVE_CARRIAGE;

				break;
			}
			case ADD:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				STACK_PUSH(&(vm.user_stack), value_A + value_B);

				MOVE_CARRIAGE;

				break;
			}
			case SUB:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				STACK_PUSH(&(vm.user_stack), value_A - value_B);

				MOVE_CARRIAGE;

				break;
			}
			case MUL:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				STACK_PUSH(&(vm.user_stack), value_A * value_B);

				MOVE_CARRIAGE;

				break;
			}
			case DIV:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				STACK_PUSH(&(vm.user_stack), value_A / value_B);

				MOVE_CARRIAGE;

				break;
			}
			case OUT:
			{
				value = STACK_POP(&(vm.user_stack)).deleted_element;

				printf("RESULT: %.3lf\n", value);

				MOVE_CARRIAGE;

				break;
			}
			case RET:
			{
				byte_code_carriage = (size_t)STACK_POP(&(vm.ret_stack)).deleted_element;

				break;
			}
			case JMP:
			{
				UPDATE_BYTE_CODE_CARRIAGE;

				break;
			}
			case JA:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				cmp_result = cmp_double(value_A, value_B);

				if(cmp_result == 1)
				{
					UPDATE_BYTE_CODE_CARRIAGE;
				}
				else
				{
					MOVE_CARRIAGE;
				}

				break;
			}
			case JB:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				cmp_result = cmp_double(value_A, value_B);

				if(cmp_result == -1)
				{
					UPDATE_BYTE_CODE_CARRIAGE;
				}
				else
				{
					MOVE_CARRIAGE;
				}

				break;
			}
			case JAE:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				cmp_result = cmp_double(value_A, value_B);

				if(cmp_result == 1 || cmp_result == 0)
				{
					UPDATE_BYTE_CODE_CARRIAGE;
				}
				else
				{
					MOVE_CARRIAGE;
				}

				break;
			}
			case JBE:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				cmp_result = cmp_double(value_A, value_B);

				if(cmp_result == -1 || cmp_result == 0)
				{
					UPDATE_BYTE_CODE_CARRIAGE;
				}
				else
				{
					MOVE_CARRIAGE;
				}

				break;
			}
			case JE:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				cmp_result = cmp_double(value_A, value_B);

				if(cmp_result == 0)
				{
					UPDATE_BYTE_CODE_CARRIAGE;
				}
				else
				{
					MOVE_CARRIAGE;
				}

				break;
			}
			case JNE:
			{
				value_B = STACK_POP(&(vm.user_stack)).deleted_element;
				value_A = STACK_POP(&(vm.user_stack)).deleted_element;

				cmp_result = cmp_double(value_A, value_B);

				if(cmp_result != 0)
				{
					UPDATE_BYTE_CODE_CARRIAGE;
				}
				else
				{
					MOVE_CARRIAGE;
				}

				break;
			}
			case CALL:
			{
				STACK_PUSH(&(vm.ret_stack), byte_code_carriage + sizeof(double));

				UPDATE_BYTE_CODE_CARRIAGE;

				break;
			}
			case HLT:
			{
				return SPU_ALL_GOOD;
			}
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
