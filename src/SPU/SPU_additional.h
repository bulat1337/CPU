#ifndef SPU_ADDITIONAL
#define SPU_ADDITIONAL

#include "SPU.h"

const int CONSTANT_IDENTIFIER_MASK = 1 << 8 * 1;
const int REGISTER_IDENTIFIER_MASK = 1 << 8 * 2;
const int RAM_IDENTIFIER_MASK      = 1 << 8 * 3;

error_t process(const char *file_name);
error_t VM_ctor(struct VM *vm);
error_t VM_dtor(struct VM *vm);

#endif
