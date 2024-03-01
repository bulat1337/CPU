#ifndef FILE_PARSE
#define FILE_PARSE

#include "assembler.h"

/**
 * @struct Buffer_w_info
 * @brief Structure representing a buffer with its length.
 */
struct Buffer_w_info
{
    char *buf; /**< Pointer to the buffer. */
    size_t length; /**< Length of the buffer. */
};

/**
 * @brief Counts the number of lines in a buffer.
 * @param buf_n_len The buffer with its length.
 * @return The number of lines in the buffer.
 */
size_t count_file_lines(struct Buffer_w_info buf_n_len);

/**
 * @brief Arranges pointers to each line in the buffer.
 * @param str_ptrs Array of pointers to each line.
 * @param buf_n_len The buffer with its length.
 * @return Returns ASM_ALL_GOOD if successful, otherwise returns an error code.
 */
error_t ptr_arranger    (char * *str_ptrs, struct Buffer_w_info buf_n_len);


#endif
