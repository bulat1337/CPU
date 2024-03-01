#ifndef SECONDARY_H
#define SECONDARY_H

#include <stdarg.h>

#define CPU_LOG(...)\
	log(__FILE__, __func__, __LINE__, __VA_ARGS__);

#define SAFE_FOR_START			\
	GLOBAL_CYCLE_COUNTER = 0;	\
	for

#define SAFE_FOR_END																	\
	GLOBAL_CYCLE_COUNTER++;																\
	if(GLOBAL_CYCLE_COUNTER >= CYCLE_LIMIT)												\
	{																					\
		fprintf(stderr, "Cycle in %s on the line %d is off the limit\n",				\
				__func__, __LINE__);													\
		break;																			\
	}
static size_t GLOBAL_CYCLE_COUNTER = 0;
const  size_t CYCLE_LIMIT          = 1000;

void   log            (const char *file_name, const char *func_name,
                       int line, const char *fmt, ...);

void   print_binary   (char *buf, size_t size, const char *buf_name);

void   clear_buffer   (void);

size_t get_file_length(FILE *file_ptr);

int    cmp_double     (double first_double, double second_double);

#endif
