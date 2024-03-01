#ifndef ASSEMBLER_H
#define ASSEMBLER_H

/**
 * @file assembler.h
 * @brief Header file containing declarations for the assembler program.
 */

#include "../global/secondary.h"

/**
 * @enum Asm_err_ID
 * @brief Enumeration of error codes for the assembler program.
 */
enum Asm_err_ID
{
    ASM_ALL_GOOD            = 0, /**< No errors occurred. */
    ASM_UNABLE_TO_OPEN_FILE = 1 << 0, /**< Unable to open file error. */
    ASM_UNABLE_TO_ALLOCATE  = 1 << 1, /**< Memory allocation error. */
    LABEL_DOESNT_EXIST      = 1 << 2, /**< Label does not exist error. */
};

typedef enum Asm_err_ID error_t; /**< Typedef for error codes. */

/**
 * @struct Buf_w_carriage_n_len
 * @brief Structure representing a buffer with carriage and length information.
 */
struct Buf_w_carriage_n_len
{
    char  *buf; /**< Pointer to the buffer. */
    size_t carriage; /**< Carriage position in the buffer. */
    size_t length; /**< Length of the buffer. */
};

/**
 * @union Second_arg
 * @brief Union representing the second argument for assembler functions.
 */
union Second_arg
{
    char *                      *strings; /**< Pointer to an array of strings. */
    FILE                        *fp; /**< Pointer to a file. */
    char                        *buf; /**< Pointer to a buffer. */
    size_t                       amount; /**< Amount value. */
    struct Buf_w_carriage_n_len  buf_w_info; /**< Buffer with carriage and length information. */
};

/**
 * @struct Universal_ret
 * @brief Structure representing the universal return type for assembler functions.
 */
struct Universal_ret
{
    error_t          error_code; /**< Error code indicating the status of the operation. */
    union Second_arg second_arg; /**< Second argument for assembler functions. */
};
typedef struct Universal_ret return_t; /**< Typedef for the universal return type. */

/**
 * @brief Compiles the human-readable code into machine code.
 * @param file_name The name of the human-readable code file.
 * @return Returns ASM_ALL_GOOD if compilation is successful, otherwise returns an error code.
 */
error_t  compile     (const char *file_name);

#endif
