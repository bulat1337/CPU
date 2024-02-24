#include <stdio.h>

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
		if(ID % 8 == 0)
		{
			CPU_LOG("double[%lu] - %lf\n", ID / 8, *(double *)(buf + ID));
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

size_t get_file_length(FILE *file_ptr)
{
    size_t length = 0;
	rewind(file_ptr);

    fseek(file_ptr, 0L, SEEK_END);
    length = (size_t)ftell(file_ptr);
    rewind(file_ptr);

    return length;
}
