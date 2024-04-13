#ifndef FILE_PARSE
#define FILE_PARSE

/**
 * @struct Buffer_w_info
 * @brief Structure representing a buffer with its length.
 */
struct Buffer_w_info
{
    char *buf; /**< Pointer to the buffer. */
    size_t length; /**< Length of the buffer. */
};

typedef struct
{
	char * *tokens;
	size_t  amount;
}Strings;

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
void ptr_arranger    (char * *str_ptrs, struct Buffer_w_info buf_n_len);

/**
 * @brief Gets the length of a file.
 *
 * Determines the length of the specified file in bytes.
 *
 * @param file_ptr Pointer to the file.
 * @return The length of the file.
 */
size_t get_file_length(FILE *file_ptr);

/**
 * @brief Parses a file and returns its content as an array of lines.
 *
 * This function reads the content of a file into a buffer, counts the number of lines,
 * and rearranges pointers to the start of each line in the buffer.
 *
 * @param file_ptr Pointer to the input file.
 * @return A Strings structure containing the parsed lines.
 */
Strings file_parse(FILE *file_ptr);


#endif
