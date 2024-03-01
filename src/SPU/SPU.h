#ifndef SPU_H
#define SPU_H

/**
 * @file SPU.h
 * @brief Header file containing function declarations for the SPU program.
 */

#include "../global/secondary.h"
#include "../global/commands.h"
#include "../../stack_src/stack.h"

const size_t AMOUNT_OF_REGISTERS = 4; /**< Number of registers in the SPU VM. */
const size_t USER_RAM_SIZE       = 400; /**< Size of user-accessible RAM in the SPU VM. */

/**
 * @struct RAM
 * @brief Structure representing the RAM of the SPU VM.
 */
struct RAM
{
    char  *byte_code; /**< Pointer to the byte code stored in RAM. */
    double user_RAM[USER_RAM_SIZE]; /**< Array representing user-accessible RAM. */
};

/**
 * @struct VM
 * @brief Structure representing the SPU VM.
 */
struct VM
{
    double registers[AMOUNT_OF_REGISTERS]; /**< Array representing registers in the SPU VM. */
    struct Stack user_stack; /**< Stack for user-defined operations. */
    struct Stack ret_stack; /**< Stack for return addresses. */
    RAM    rand_access_mem; /**< Random access memory in the SPU VM. */
};

/**
 * @enum Asm_err_ID
 * @brief Enumeration of error codes for the SPU program.
 */
enum Asm_err_ID
{
    SPU_ALL_GOOD            = 0, /**< No errors occurred. */
    SPU_UNABLE_TO_OPEN_FILE = 1 << 0, /**< Unable to open file error. */
    SPU_UNABLE_TO_ALLOCATE  = 1 << 1, /**< Memory allocation error. */
    INVALID_FREAD           = 1 << 2, /**< Invalid read operation error. */
    INVALID_RAM_MODE        = 1 << 3, /**< Invalid RAM access mode error. */
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
 * @brief Union representing the second argument for SPU functions.
 */
union Second_arg
{
    FILE * file_ptr; /**< Pointer to a file. */
};

/**
 * @struct Universal_ret
 * @brief Structure representing the universal return type for SPU functions.
 */
struct Universal_ret
{
    error_t          error_code; /**< Error code indicating the status of the operation. */
    union Second_arg second_arg; /**< Second argument for SPU functions. */
};
typedef struct Universal_ret return_t; /**< Typedef for the universal return type. */

/**
 * @brief Executes the SPU program.
 * @param file_name Name of the file containing the SPU byte code.
 * @return Returns a structure containing the execution result.
 */
return_t execute(const char *file_name);

#endif
