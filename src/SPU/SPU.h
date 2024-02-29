#ifndef SPU_H
#define SPU_H

#include "../global/secondary.h"
#include "../global/commands.h"
#include "../../stack_src/stack.h"

const size_t AMOUNT_OF_REGISTERS = 4;
const size_t USER_RAM_SIZE       = 400;

struct RAM
{
	char *byte_code;
	double user_RAM[USER_RAM_SIZE];
};

struct VM
{
	double registers[AMOUNT_OF_REGISTERS];
	struct Stack user_stack;
	struct Stack ret_stack;
	RAM rand_access_mem;
};

enum Asm_err_ID
{
	SPU_ALL_GOOD            = 0,
	SPU_UNABLE_TO_OPEN_FILE = 1 << 0,
	SPU_UNABLE_TO_ALLOCATE  = 1 << 1,
	INVALID_FREAD           = 1 << 2,
	INVALID_RAM_MODE        = 1 << 3,
};

typedef enum Asm_err_ID error_t;

struct Buf_w_carriage_n_len
{
	char  *buf;
	size_t carriage;
	size_t length;
};

union Second_arg
{
	FILE * file_ptr;
};

struct Universal_ret
{
	error_t          error_code;
	union Second_arg second_arg;
};

typedef struct Universal_ret return_t;

return_t execute(const char *file_name);

#endif
