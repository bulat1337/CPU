#ifndef DRIVERS_H
#define DRIVERS_H

// for some reason cant just do #include "SPU.h"
#include "SPU.h"

/**
 * @brief Draws the content of the virtual machine's user RAM to a file.
 *
 * This function reads the specified range of addresses from the virtual machine's
 * user RAM and prints the corresponding characters to the specified output file.
 * The characters are printed in a grid format to represent the content.
 *
 * @param vm Pointer to the virtual machine.
 * @param current_byte_code Pointer to the current byte code.
 * @param output_file Pointer to the output file where the content will be printed.
 */
void file_draw(VM *vm, char *current_byte_code, FILE *output_file);

/**
 * @brief Draws the content of the virtual machine's user RAM to an SFML window.
 *
 * This function reads the specified range of addresses from the virtual machine's
 * user RAM and draws the corresponding pixels to an SFML window. The pixels are
 * drawn as red rectangles on a black background to represent the content graphically.
 *
 * @param vm Pointer to the virtual machine.
 * @param current_byte_code Pointer to the current byte code.
 * @param junk Unused parameter (can be NULL).
 */
void window_draw(VM *vm, char * current_byte_code, FILE *junk);

#endif
