#include <stdio.h>
#include <math.h>
#include <string.h>

#include "secondary.h"

void log(const char *file_name, const char *func_name, int line, const char *fmt, ...)
{

    static FILE *log_file = fopen("log.txt", "w");

    if (log_file == NULL)
	{
        perror("Error opening log_file");
        return;
    }

    va_list args = NULL;

    va_start(args, fmt);

	// fprintf(log_file, "file: %s func: %s on line : %d\n", file_name, func_name, line);
    vfprintf(log_file, fmt, args);

    va_end(args);
}

void print_binary(char *buf, size_t size, const char *buf_name)
{
	CPU_LOG("%s:\n", buf_name);
    for (size_t ID = 0; ID < size; ID++)
	{
		if(ID % sizeof(double) == 0)
		{
			CPU_LOG("double[%lu] - %lf\n", ID / sizeof(double), *(double *)(buf + ID));
		}

        char cur_elem = buf[ID];
		CPU_LOG("[%2.lu] - ", ID);

        for (int bit_offset = 7; bit_offset >= 0; bit_offset--)
		{
            CPU_LOG("%d", (cur_elem >> bit_offset) & 1);
        }

        CPU_LOG("\n");
    }
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

void clear_buffer(void)
{
    while(getchar() != '\n');
}

size_t max_len(const char *str_1, const char *str_2)
{
	size_t size_1 = strlen(str_1);
	size_t size_2 = strlen(str_2);
	if(size_1 > size_2)
	{
		return size_1;
	}
	else
	{
		return size_2;
	}
}
