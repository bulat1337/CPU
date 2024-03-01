#ifndef SPU_ADDITIONAL
#define SPU_ADDITIONAL

/**
 * @file SPU_additional.h
 * @brief Additional functions for the SPU program.
 */

#include "SPU.h"

const int CONSTANT_IDENTIFIER_MASK = 1 << 8 * 1;
const int REGISTER_IDENTIFIER_MASK = 1 << 8 * 2;
const int RAM_IDENTIFIER_MASK      = 1 << 8 * 3;

/**
 * @brief Processes the input file and performs operations based on the byte code.
 * @param input_file Pointer to the input file.
 * @param output_file Pointer to the output file.
 * @return Returns SPU_ALL_GOOD if successful, otherwise returns an error code.
 */
error_t process(FILE *input_file, FILE *output_file);

/**
 * @brief Constructor for the Virtual Machine.
 * @param vm Pointer to the Virtual Machine structure.
 * @return Returns SPU_ALL_GOOD if successful, otherwise returns an error code.
 */
error_t VM_ctor(struct VM *vm);

/**
 * @brief Destructor for the Virtual Machine.
 * @param vm Pointer to the Virtual Machine structure.
 * @return Returns SPU_ALL_GOOD if successful, otherwise returns an error code.
 */
error_t VM_dtor(struct VM *vm);

#endif
