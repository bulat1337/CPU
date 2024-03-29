#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "SPU_additional.h"
#include "file_parse.h"

/**
 * @def BYTE_CODE
 * @brief Macro representing the byte code of the Virtual Machine.
 */
#define BYTE_CODE vm.byte_code

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

#define ALLOCATION_CHECK(ptr)					\
	if(ptr == NULL)								\
	{											\
		CPU_LOG("Unable to allocate"#ptr".\n");	\
		return SPU_UNABLE_TO_ALLOCATE;			\
	}

#define CALLOC(ptr, amount, type)				\
	ptr = (type *)calloc(amount, sizeof(type));	\
	ALLOCATION_CHECK(ptr);

error_t process(FILE *bin_file, const char *config_file,
				FILE *output_file, void (*driver)(VM *, char *, FILE *))
{
	size_t byte_code_length = get_file_length(bin_file);
	error_t error_code = SPU_ALL_GOOD;

	VM_CTOR(vm, config_file);

	CALLOC(BYTE_CODE, byte_code_length, char);

	FREAD(BYTE_CODE, sizeof(char), byte_code_length, bin_file);

	size_t byte_code_carriage = 0;
	char command              = (char)VOID;
	size_t reg_type           = 0;
	elem_t user_entered_value = NAN;
	elem_t value              = NAN;
	elem_t value_A            = NAN;
	elem_t value_B            = NAN;
	int cmp_result            = 666;
	unsigned int RAM_address  = 0;

	bool run_flag = false;

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
		#ifdef CPU_DEBUG
			debug(&vm, &run_flag, command);
		#endif

	}

	free(BYTE_CODE);
	VM_dtor(&vm);

	return SPU_ALL_GOOD;
}

#undef CMD_DEF

error_t VM_ctor(struct VM *vm, const char *config_file)
{
	WITH_OPEN
	(
		config_file, "r", config_file_ptr,

		Strings settings = file_parse(config_file_ptr);
	)
	if(settings.tokens == NULL)
	{
		return SPU_INVALID_PARSE;
	}

	#define IS_SETTING(setting)\
		!strncmp(settings.tokens[set_ID], setting, LEN(setting))

	size_t regs_amount = 0;
	size_t RAM_size    = 0;

	for(size_t set_ID = 0; set_ID < settings.amount; set_ID++)
	{
		if(IS_SETTING("regs_amount:"))
		{
			sscanf(settings.tokens[set_ID], "%*[^:]%*2c%lu", &regs_amount);
			vm->regs_amount = regs_amount;

			CPU_LOG("regs amount = %lu\n", regs_amount);
		}
		else if(IS_SETTING("RAM_size:"))
		{
			sscanf(settings.tokens[set_ID], "%*[^:]%*2c%lu", &RAM_size);
			vm->rand_access_mem.RAM_size = RAM_size;

			CPU_LOG("ram size = %lu\n", RAM_size);
		}
	}

	CALLOC(vm->registers, regs_amount, elem_t);
	CALLOC(vm->rand_access_mem.user_RAM, RAM_size, elem_t);

	STACK_CTOR(&(vm->user_stack), STD_USER_STACK_SIZE);
	STACK_CTOR(&(vm->ret_stack),  STD_RET_STACK_SIZE);

	return SPU_ALL_GOOD;
}

error_t VM_dtor(struct VM *vm)
{
	free(vm->rand_access_mem.user_RAM);
	free(vm->registers);

	vm->rand_access_mem.RAM_size = 0;

	stack_dtor(&(vm->user_stack));
	stack_dtor(&(vm->ret_stack));

	return SPU_ALL_GOOD;
}

#undef UPDATE_BYTE_CODE_CARRIAGE
#undef MOVE_CARRIAGE

#ifdef CPU_DEBUG

char get_db_opt(void)
{
	char answer = 0;

	printf("[r] to run\n");
	printf("[c] to continue\n");

	printf("> ");
	scanf("%c", &answer);

	clear_buffer();

	return answer;
}

bool proc_db_opt(char answer)
{
	bool run_flag = false;

	switch(answer)
	{
		case 'c':
		{
			break;
		}
		case 'r':
		{
			run_flag = true;
			break;
		}
		default:
		{
			printf("WARNING: Symbols were ignored. Continuing the program\n");
			break;
		}
	}

	return run_flag;
}

void dump_vm(VM *vm)
{
	//dump regs
	printf("    REGISTERS\n");
	for(unsigned char reg_ID = 0; reg_ID < vm->regs_amount; reg_ID++)
	{
		printf("[r%cx]: ", reg_ID + 'a');
		printf("%lf\n", vm->registers[reg_ID]);
	}
	printf("\n");

	if(vm->user_stack.size != 0)
	{
		printf("    USER_STACK\n");
		dump_stack(&(vm->user_stack));
	}

	if(vm->ret_stack.size != 0)
	{
		printf("    RET_STACK\n");
		dump_stack(&(vm->ret_stack));
	}
}

#define CASE(cmd)								\
	case cmd:									\
	{											\
		printf("Current command: "#cmd"\n");	\
		break;									\
	}

void dump_cmd(char command)
{
	switch(command)
	{
		CASE(VOID)
		CASE(PUSH)
		CASE(ADD )
		CASE(SUB )
		CASE(MUL )
		CASE(DIV )
		CASE(OUT )
		CASE(IN  )
		CASE(POP )
		CASE(JMP )
		CASE(JA  )
		CASE(JB  )
		CASE(JAE )
		CASE(JBE )
		CASE(JE  )
		CASE(JNE )
		CASE(CALL)
		CASE(RET )
		CASE(DRAW)
		CASE(SQRT)
		CASE(HLT )
		default:
		{
			printf("Unknown command\n");
		}
	}
}

#undef CASE

void dump_stack(Stack *stk)
{
	for(size_t stk_ID = 0; stk_ID < stk->size; stk_ID++)
	{
		printf("[%zu]: ", stk_ID);
		printf("%lf\n", stk->data[stk_ID]);
	}
	printf("\n");
}

void frame_terminal(size_t amount)
{
	for(size_t ID = 0; ID < amount; ID++)
	{
		printf("-");
	}
	printf("\n");
}

void debug(VM *vm, bool *run_flag, char command)
{
	char answer = 0;

	if(*run_flag == false)
	{
		dump_cmd(command);

		dump_vm(vm);

		answer   = get_db_opt();

		*run_flag = proc_db_opt(answer);

		frame_terminal(30);
	}
}

#endif
