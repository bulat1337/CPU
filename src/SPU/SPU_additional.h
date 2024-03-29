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
#define CHECK_ERROR\
	if(error_code != SPU_ALL_GOOD)\
		return error_code;

#define CALL(...)				\
	error_code = __VA_ARGS__;	\
	CHECK_ERROR;

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

#define VM_CTOR(name, config_file)			\
		struct VM name = {};				\
		CALL(VM_ctor(&name, config_file));

#define FILE_PTR_CHECK(file_ptr)                                    \
    if(file_ptr == NULL)                                            \
    {                                                               \
        CPU_LOG("\nERROR: Unable to open "#file_ptr"\n");           \
        return SPU_UNABLE_TO_OPEN_FILE;								\
	}

const size_t STD_USER_STACK_SIZE = 10;
const size_t STD_RET_STACK_SIZE  = 2;

error_t process(FILE *bin_file, const char *config_file,
				FILE *output_file, void (*driver)(VM *, char *, FILE *));

error_t VM_ctor(struct VM *vm, const char *config_file);

/**
 * @brief Destructor for the Virtual Machine.
 * @param vm Pointer to the Virtual Machine structure.
 * @return Returns SPU_ALL_GOOD if successful, otherwise returns an error code.
 */
error_t VM_dtor(struct VM *vm);

#ifdef CPU_DEBUG
	char get_db_opt(void);

	bool proc_db_opt(char answer);

	void dump_vm(VM *vm);

	void dump_cmd(char command);

	void dump_stack(Stack *stk);

	void frame_terminal(size_t amount);

	void debug(VM *vm, bool *run_flag, char command);
#endif

#endif
