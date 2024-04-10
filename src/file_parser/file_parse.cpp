#include <stdio.h>
#include <stdlib.h>

#include "file_parse.h"

/**
 * @brief Checks if a pointer is NULL and prints an error message if it is.
 *
 * If the pointer is NULL, an error message is printed to the standard error stream,
 * and an empty Strings structure is returned.
 *
 * @param ptr Pointer to check.
 * @return An empty Strings structure if the pointer is NULL.
 */
#define ALLOCATION_CHECK(ptr)								\
	if(ptr == NULL)											\
	{														\
		fprintf(stderr, "Unable to allocate "#ptr".\n");	\
		Strings void_strs = {};								\
		return void_strs;									\
	}

/**
 * @brief Allocates memory for an array and checks if the allocation was successful.
 *
 * This macro allocates memory for an array of a specified type and size. If the
 * allocation fails, an error message is printed, and an empty Strings structure
 * is returned.
 *
 * @param ptr Pointer to the allocated memory.
 * @param amount Number of elements to allocate.
 * @param type Type of the elements.
 */
#define CALLOC(ptr, amount, type)				\
	ptr = (type *)calloc(amount, sizeof(type));	\
	ALLOCATION_CHECK(ptr);

/**
 * @brief Reads data from a file and checks if the expected amount of data was read.
 *
 * This macro reads a specified amount of elements from a file into a buffer. If the
 * actual number of elements read does not match the expected amount, an error message
 * is printed, and an empty Strings structure is returned.
 *
 * @param buf Pointer to the buffer where the data will be read.
 * @param elem_size Size of each element to read.
 * @param amount Expected number of elements to read.
 * @param file_ptr Pointer to the input file.
 */
#define FREAD(buf, elem_size, amount, file_ptr)								\
	size_t read_elems = fread(buf, elem_size, amount, file_ptr);			\
	if(read_elems != amount)												\
	{																		\
		fprintf(stderr, "ERROR: fread read unexpected amount of elems.\n");	\
		fprintf(stderr, "\t expected amount: %lu.\n", amount);				\
		fprintf(stderr, "\t read amount: %lu.\n", read_elems);				\
																			\
		Strings void_strs = {};												\
		return void_strs;													\
	}

size_t count_file_lines(struct Buffer_w_info buf_n_len)
{
	size_t amount_of_lines = 0;
	for (size_t buf_ID = 0; buf_ID < buf_n_len.length; buf_ID++)
    {
        if(buf_n_len.buf[buf_ID] == '\n')
        {
            amount_of_lines++;
        }
    }

	return amount_of_lines;
}

void ptr_arranger(char * *str_ptrs, struct Buffer_w_info buf_n_len)
{
	str_ptrs[0] = buf_n_len.buf;
	size_t line_ID = 1;
    for (size_t buf_ID = 0; buf_ID < buf_n_len.length; buf_ID++)
    {
        if(buf_n_len.buf[buf_ID] == '\n')
        {
            buf_n_len.buf[buf_ID] = '\0';
			if(buf_ID + 1 < buf_n_len.length)
			{
            	str_ptrs[line_ID] = &(buf_n_len.buf[buf_ID + 1]);
            	line_ID++;
			}
        }
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

Strings file_parse(FILE *file_ptr)
{
	Buffer_w_info buf_w_len = {};

	buf_w_len.length = get_file_length(file_ptr);

	if(buf_w_len.length == 0)
	{
		Strings void_strs = {};
		return void_strs;
	}

	CALLOC(buf_w_len.buf, buf_w_len.length, char);

	FREAD(buf_w_len.buf, sizeof(char), buf_w_len.length, file_ptr);


	Strings strs = {};

	strs.amount = count_file_lines(buf_w_len);

	CALLOC(strs.tokens, strs.amount, char *);

	ptr_arranger(strs.tokens,  buf_w_len);


	return strs;
}

