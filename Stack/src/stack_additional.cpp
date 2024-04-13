#include <stdio.h>
#include <math.h>

#include "stack_additional.h"

void show_bits(int x, FILE *log_file)
{
    int i = 0;
    for (i = (sizeof(int) * 8) - 1; i >= 0; i--)
    {
       fputc(x & (1 << i) ? '1' : '0', log_file);
	   if(i % 4 == 0)
	   {
			fputc(' ', log_file);
	   }
    }
    fprintf(log_file ,"\n");
}
