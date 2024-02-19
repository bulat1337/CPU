#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "../global/secondary.h"

enum Asm_err_ID
{
	ASM_ALL_GOOD            = 0,
	ASM_UNABLE_TO_OPEN_FILE = 1 << 0,
	ASM_UNABLE_TO_ALLOCATE  = 1 << 1,
	LABEL_DOESNT_EXIST     = 1 << 2,
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
	char *                      *strings;
	FILE                        *fp;
	char                        *buf;
	size_t                       amount;
	struct Buf_w_carriage_n_len  buf_w_info;
};

struct Universal_ret
{
	error_t          error_code;
	union Second_arg second_arg;
};

typedef struct Universal_ret return_t;

error_t compile(const char *file_name);
return_t cmds_process(char * *commands);


#endif
