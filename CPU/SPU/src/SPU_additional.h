#ifndef SPU_ADDITIONAL
#define SPU_ADDITIONAL

/**
 * @file SPU_additional.h
 * @brief Additional functions for the SPU program.
 */

#include "SPU.h"

/**
 * @def CHECK_ERROR(result)
 * @brief Macro to check if an error occurred during a function call.
 * @param result The result structure to check for errors.
 * @return Returns the error code if an error occurred, otherwise continues execution.
 */
#define CHECK_ERROR					\
	if(error_code != SPU_ALL_GOOD)	\
		return error_code;

/**
 * @def CALL(...)
 * @brief Macro to call a function and check its return value.
 *
 * This macro is used to call a function and check its return value for errors.
 * If an error occurs, it sets the error code and checks the error.
 *
 * @param ... The function call to be executed.
 */
#define CALL(...)				\
	error_code = __VA_ARGS__;	\
	CHECK_ERROR;

/**
 * @def FREAD(buf, elem_size, amount, file_ptr)
 * @brief Macro to read from a file and check the number of read elements.
 *
 * This macro reads a specified amount of elements from a file into a buffer.
 * It then checks if the expected number of elements were read and logs an error
 * if the read operation is not successful.
 *
 * @param buf Pointer to the buffer to store the read elements.
 * @param elem_size Size of each element to read.
 * @param amount Number of elements to read.
 * @param file_ptr Pointer to the file to read from.
 */
#define FREAD(buf, elem_size, amount, file_ptr)\
	size_t read_elems = fread(buf, elem_size, amount, file_ptr);	\
	if(read_elems != amount)										\
	{																\
		CPU_LOG("ERROR: fread read unexpected amount of elems.\n");	\
		CPU_LOG("\t expected amount: %lu.\n", amount);				\
		CPU_LOG("\t read amount: %lu.\n", read_elems);				\
																	\
		return SPU_INVALID_FREAD;									\
	}

/**
 * @def VM_CTOR(name, config_file)
 * @brief Macro to initialize a VM and check its constructor return value.
 *
 * This macro initializes a VM structure and checks the return value of its constructor.
 * If the constructor fails, it logs an error and returns an appropriate error code.
 *
 * @param name Name of the VM structure to initialize.
 * @param config_file Configuration file to use for VM initialization.
 */
#define VM_CTOR(name, config_file)			\
		struct VM name = {};				\
		CALL(VM_ctor(&name, config_file));

/**
 * @def FILE_PTR_CHECK(file_ptr)
 * @brief Macro to check if a file pointer is valid.
 *
 * This macro checks if a given file pointer is valid. If the file pointer is NULL,
 * it logs an error and returns an appropriate error code.
 *
 * @param file_ptr Pointer to the file to check.
 */
#define FILE_PTR_CHECK(file_ptr)                                    \
    if(file_ptr == NULL)                                            \
    {                                                               \
        CPU_LOG("\nERROR: Unable to open "#file_ptr"\n");           \
        return SPU_UNABLE_TO_OPEN_FILE;								\
	}

const size_t STD_USER_STACK_SIZE = 10;
const size_t STD_RET_STACK_SIZE  = 2;

/**
 * @brief Processes the byte code.
 *
 * @param bin_file Pointer to the binary file containing the byte code.
 * @param config_file Pointer to the configuration file.
 * @param output_file Pointer to the output file.
 * @param driver Pointer to the function driver.
 * @return error_t Returns an error code indicating the status of the processing.
 */
error_t process(FILE *bin_file, const char *config_file,
				FILE *output_file, void (*driver)(VM *, char *, FILE *));


/**
 * @brief Virtual Machine constructor.
 *
 * @param vm Pointer to the Virtual Machine structure.
 * @param config_file Pointer to the configuration file.
 * @return error_t Returns an error code indicating the status of the constructor.
 */
error_t VM_ctor(struct VM *vm, const char *config_file);

/**
 * @brief Destructor for the Virtual Machine.
 * @param vm Pointer to the Virtual Machine structure.
 * @return Returns SPU_ALL_GOOD if successful, otherwise returns an error code.
 */
error_t VM_dtor(struct VM *vm);

#ifdef CPU_DEBUG
	/**
	* @brief Gets the debug option from the user.
	*
	* @return char Returns the selected debug option.
	*/
	char get_db_opt(void);

	/**
	* @brief Processes the debug option.
	*
	* @param answer Debug option selected by the user.
	* @return bool Returns a boolean indicating whether to run the debug option or not.
	*/
	bool proc_db_opt(char answer);

	/**
	* @brief Dumps the current state of the Virtual Machine.
	*
	* @param vm Pointer to the Virtual Machine structure.
	*/
	void dump_vm(VM *vm);

	/**
	* @brief Dumps the command based on the provided command value.
	*
	* @param command The command value to be dumped.
	*/
	void dump_cmd(char command);

	/**
	* @brief Dumps the command based on the provided command value.
	*
	* @param command The command value to be dumped.
	*/
	void dump_stack(Stack *stk);

	/**
	* @brief Prints a frame of terminal dashes.
	*
	* @param amount Number of dashes to print.
	*/
	void frame_terminal(size_t amount);

	/**
	* @brief Provides a debug view of the Virtual Machine's current state.
	*
	* @param vm Pointer to the Virtual Machine.
	* @param run_flag Pointer to the flag indicating the debug run status.
	* @param command Current command being processed.
	*/
	void debug(VM *vm, bool *run_flag, char command);
#endif

#endif
