#include <stdio.h>
#include <math.h>

#include "drivers.h"

void terminal_draw(VM *vm, char *current_byte_code, FILE *output_file)
{
	size_t screen_size = (size_t)sqrt(vm->RAM_size); // sqrt тяжело

	unsigned int head = *(unsigned int *)(current_byte_code + sizeof(double));
	unsigned int end  = *(unsigned int *)(current_byte_code + sizeof(double) + sizeof(int));

	for(unsigned int address = head; address < end; address++)
	{
		fprintf(output_file, "%c  ", (char)vm->rand_access_mem.user_RAM[address]);

		if((address + 1) % screen_size == 0)
		{
			fprintf(output_file, "\n");
		}

	}
}
