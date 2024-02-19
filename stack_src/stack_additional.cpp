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

int cmp_double(double first_double, double second_double)
{
    const double eps = 1e-7;

    if(isnan(first_double) && isnan(second_double))
    {
        return 0;
    }
    if (fabs(first_double - second_double) < eps)
    {
        return 0;
    }
    else if ((first_double - second_double) > eps)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}
