#ifndef SPU_H
#define SPU_H

#include <stdio.h>

#include "../global/secondary.h"

enum Asm_err_ID
{
	SPU_ALL_GOOD            = 0,
	SPU_UNABLE_TO_OPEN_FILE = 1 << 0,
	SPU_UNABLE_TO_ALLOCATE  = 1 << 1,
};

typedef enum Asm_err_ID error_t;

struct Buf_w_carriage_n_len
{
	char  *buf;
	size_t carriage;
	size_t length;
};

error_t execute(const char *file_name);

#endif
