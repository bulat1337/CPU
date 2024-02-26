#ifndef SECONDARY_H
#define SECONDARY_H

#include <stdarg.h>

#define CPU_LOG(...)\
	log(__FILE__, __func__, __LINE__, __VA_ARGS__);

void log(const char *file_name, const char *func_name, int line, const char *fmt, ...);
size_t get_file_length(FILE *file_ptr);
void print_binary(char *buf, size_t size, const char *buf_name);
int cmp_double(double first_double, double second_double);
void clear_buffer(void);

#endif
