#ifndef DRIVERS_H
#define DRIVERS_H

// for some reason cant just do #include "SPU.h"
#include "../SPU/SPU.h"

void terminal_draw(VM *vm, char *current_byte_code, FILE *output_file);

#endif
