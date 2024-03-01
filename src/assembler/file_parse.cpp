#include <stdio.h>

#include "file_parse.h"

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

error_t ptr_arranger(char * *str_ptrs, struct Buffer_w_info buf_n_len)
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

	return ASM_ALL_GOOD;
}

