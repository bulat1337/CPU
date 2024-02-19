#ifndef FILE_PARSE
#define FILE_PARSE

struct Buffer_w_info
{
	char *buf;
	size_t length;
};

size_t count_file_lines(struct Buffer_w_info buf_n_len);
void ptr_arranger(char * *str_ptrs, struct Buffer_w_info buf_n_len);


#endif
