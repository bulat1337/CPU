#ifndef SPU_H
#define SPU_H

#define CPU_DEBUG

/**
 * @file SPU.h
 * @brief Header file containing function declarations for the SPU program.
 */

#include "secondary.h"
#include "commands.h"
#include "stack.h"

const size_t AMOUNT_OF_REGISTERS = 4; /**< Number of registers in the SPU VM. */
const size_t USER_RAM_SIZE       = 400; /**< Size of user-accessible RAM in the SPU VM. */

/**
 * @struct RAM
 * @brief Structure representing the RAM of the SPU VM.
 */
struct RAM
{
    size_t RAM_size;
    elem_t *user_RAM; /**< Array representing user-accessible RAM. */
};

/**
 * @struct VM
 * @brief Structure representing the SPU VM.
 */
struct VM
{
    elem_t *registers; /**< Array representing registers in the SPU VM. */
    struct Stack user_stack; /**< Stack for user-defined operations. */
    struct Stack ret_stack; /**< Stack for return addresses. */
    RAM    rand_access_mem; /**< Random access memory in the SPU VM. */
	char  *byte_code; /**< Pointer to the byte code. */
	size_t regs_amount;
};

/**
 * @enum Asm_err_ID
 * @brief Enumeration of error codes for the SPU program.
 */
typedef enum
{
    SPU_ALL_GOOD            = 0, /**< No errors occurred. */
    SPU_UNABLE_TO_OPEN_FILE = 1 << 0, /**< Unable to open file error. */
    SPU_UNABLE_TO_ALLOCATE  = 1 << 1, /**< Memory allocation error. */
    SPU_INVALID_FREAD       = 1 << 2, /**< Invalid read operation error. */
    INVALID_RAM_MODE        = 1 << 3, /**< Invalid RAM access mode error. */
    SPU_INVALID_PARSE       = 1 << 4, /**< Parse_file executed with an error. */
} error_t;

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
typedef struct
{
    error_t          error_code; /**< Error code indicating the status of the operation. */
    union Second_arg second_arg; /**< Second argument for SPU functions. */
} return_t;

/**
 * @brief Executes the provided binary file using the specified configuration and driver.
 *
 * This function opens the binary and configuration files, then calls the process function
 * to execute the binary instructions. The results of the execution are written to "execution_result.txt".
 *
 * @param bin_file Path to the binary file to execute.
 * @param config_file Path to the configuration file.
 * @param driver Pointer to the driver function for the Virtual Machine.
 * @return An error code indicating the success or failure of the execution.
 */
error_t execute(const char *bin_file, const char *config_file,
				void (*driver)(VM *, char *, FILE *));

#endif
