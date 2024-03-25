#ifndef SECONDARY_H
#define SECONDARY_H

/**
 * @file secondary.h
 * @brief Header file containing declarations of secondary functions.
 */

#include <stdarg.h>

#define WITH_OPEN(file_name, mode, file_ptr, ...)	\
	FILE *file_ptr = fopen(file_name, mode);		\
	FILE_PTR_CHECK(file_ptr);						\
													\
	__VA_ARGS__										\
													\
	fclose(file_ptr);

#define LEN(token)\
	sizeof(token) / sizeof(char) - 1

/**
 * @brief Macro to log messages to a file.
 *
 * This macro simplifies logging by automatically passing file name, function name, and line number.
 * Usage: CPU_LOG("Message to log");
 */
#define CPU_LOG(...)\
	log(__FILE__, __func__, __LINE__, __VA_ARGS__);

/**
 * @brief Macro to start a loop with safety check.
 *
 * This macro initializes a global loop counter and starts a loop with a safety check against excessive iterations.
 * Usage: SAFE_FOR_START { ... } SAFE_FOR_END
 */
#define SAFE_FOR_START			\
	GLOBAL_CYCLE_COUNTER = 0;	\
	for

/**
 * @brief Macro to end a loop with a safety check against excessive iterations.
 *
 * This macro increments the loop counter and checks if it exceeds a predefined limit.
 * If the limit is reached, it prints an error message and breaks the loop.
 * Usage: SAFE_FOR_START { ... } SAFE_FOR_END
 */
#define SAFE_FOR_END																	\
	GLOBAL_CYCLE_COUNTER++;																\
	if(GLOBAL_CYCLE_COUNTER >= CYCLE_LIMIT)												\
	{																					\
		fprintf(stderr, "Cycle in %s on the line %d is off the limit\n",				\
				__func__, __LINE__);													\
		break;																			\
	}

static size_t  GLOBAL_CYCLE_COUNTER      = 0; /**< Global counter for loop iterations. */
const  size_t  CYCLE_LIMIT               = 1000; /**< Limit for loop iterations. */
const  char    RAM_MASK                  = (const char)1 << 7;
const  char    REG_MASK                  = (const char)1 << 6;
const  char    IMM_MASK                  = (const char)1 << 5;

/**
 * @brief Logs a message to a file.
 *
 * Logs a message to a file named "log.txt" with the specified format and additional information.
 *
 * @param file_name The name of the file where the log message originates.
 * @param func_name The name of the function where the log message originates.
 * @param line The line number in the source file where the log message originates.
 * @param fmt The format string for the log message.
 * @param ... Additional arguments to be formatted according to the format string.
 */
void log(const char *file_name, const char *func_name, int line, const char *fmt, ...);

/**
 * @brief Prints the binary representation of a buffer.
 *
 * Prints the binary representation of the specified buffer, interpreting its contents as individual bytes.
 *
 * @param buf Pointer to the buffer.
 * @param size Size of the buffer.
 * @param buf_name Name of the buffer for identification purposes.
 */
void print_binary(char *buf, size_t size, const char *buf_name);

/**
 * @brief Compares two double-precision floating-point numbers.
 *
 * Compares two double-precision floating-point numbers with a small epsilon tolerance.
 *
 * @param first_double The first double to compare.
 * @param second_double The second double to compare.
 * @return 0 if the two numbers are considered equal, 1 if the first is greater, -1 if the second is greater.
 */
int cmp_double(double first_double, double second_double);

/**
 * @brief Clears the input buffer.
 *
 * Clears the input buffer by reading and discarding characters until a newline character is encountered.
 */
void clear_buffer(void);

size_t max_len(const char *str_1, const char *str_2);

#endif
