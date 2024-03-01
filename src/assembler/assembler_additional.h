#ifndef ASSEMBLER_ADDITIONAL
#define ASSEMBLER_ADDITIONAL

/**
 * @file assembler_additional.h
 * @brief Header file containing additional functions and structures for the assembler program.
 */

#include "assembler.h"
#include "file_parse.h"

/**
 * @struct Label
 * @brief Structure representing a label with its name and position.
 */
struct Label
{
    char  *name; /**< Name of the label. */
    size_t IP_pos; /**< Position of the label in the bytecode. */
};

/**
 * @struct Labels_w_carriage
 * @brief Structure representing labels along with their carriage position.
 */
struct Labels_w_carriage
{
    Label *labels; /**< Array of labels. */
    size_t carriage; /**< Carriage position of the labels. */
};

/**
 * @struct JMP_pos
 * @brief Structure representing a jump with its name and position.
 */
struct JMP_pos
{
    char const *name; /**< Name of the jump. */
    int         IP_pos; /**< Position of the jump in the bytecode. */
};

/**
 * @struct JMP_poses_w_carriage
 * @brief Structure representing jumps along with their carriage position.
 */
struct JMP_poses_w_carriage
{
    JMP_pos *JMP_poses; /**< Array of jumps. */
    size_t   carriage; /**< Carriage position of the jumps. */
};

/**
 * @struct Parse_human_code_result
 * @brief Structure representing the result of parsing human-readable code.
 */
struct Parse_human_code_result
{
    error_t error_code; /**< Error code indicating the status of the parsing operation. */
    char * *strings; /**< Array of strings representing parsed commands. */
    size_t  amount_of_lines; /**< Number of lines in the human-readable code. */
    struct  Buffer_w_info human_code_buffer; /**< Buffer with carriage and length information for human-readable code. */
};

/**
 * @struct Cmds_process_result
 * @brief Structure representing the result of processing commands.
 */
struct Cmds_process_result
{
    error_t                     error_code; /**< Error code indicating the status of command processing. */
    struct Buf_w_carriage_n_len buf_w_info; /**< Buffer with carriage and length information for bytecode. */
    struct Labels_w_carriage    labels_w_carriage; /**< Structure containing labels with carriage information. */
    struct JMP_poses_w_carriage jmp_poses_w_carriage; /**< Structure containing jumps with carriage information. */
};

const int           POISON_JMP_POS           = -1;
const char          IDENTIFIER_BYTE          =  1;
const int           CMD_TYPE_ALIGNMENT_VALUE =  3;
const size_t        ALIGN_TO_INT             =  sizeof(int) - sizeof(char);
const size_t        ALIGN_TO_DOUBLE          =  sizeof(double) - sizeof(char);
const char * const  MAIN_JMP_NAME            = "main";
const unsigned char SPACE_SKIP               = 1;
const unsigned char LETTER_SKIP              = 1;
const unsigned char SIX_BYTE_ALIGNMENT       = 6;
const unsigned char ONE_BYTE_ALIGNMENT       = 1;

/**
 * @brief Parses human-readable code.
 *
 * Parses the given human-readable code file and returns the parsed result.
 *
 * @param file_name Name of the file containing human-readable code.
 * @return Parsed result including error code, human code buffer, etc.
 */
struct Parse_human_code_result parse_human_code(const char *file_name);

/**
 * @brief Processes commands.
 *
 * Processes commands of the given human-readable code.
 *
 * @param commands Array of command strings.
 * @param amount_of_lines Number of lines in the code.
 * @return Result of command processing including bytecode, labels, jumps, etc.
 */
struct Cmds_process_result cmds_process(char * *commands, size_t amount_of_lines);

/**
 * @brief Writes main jump instruction.
 *
 * Writes the main jump instruction to the bytecode.
 *
 * @param byte_code Pointer to the bytecode buffer.
 * @param jmp_poses_w_carriage Pointer to the structure containing jump positions.
 * @return Error code indicating the success or failure of the operation.
 */
error_t write_main_jmp(struct Buf_w_carriage_n_len *byte_code,
					   JMP_poses_w_carriage *jmp_poses_w_carriage);

/**
 * @brief Writes data of a certain size to the buffer.
 *
 * @param byte_code Pointer to Buf_w_carriage_n_len structure representing buffer with position and length information.
 * @param value Pointer to the data to be written.
 * @param size Size of the data in bytes.
 * @return error_t Error code indicating the success of the operation.
 */
error_t write_to_buf(struct Buf_w_carriage_n_len *byte_code,
                     const void *value, size_t size);

/**
 * @brief Adds alignment to a buffer by adding "void" bytes equivalent to zero.
 *
 * @param buf Pointer to Buf_w_carriage_n_len structure representing buffer with position and length information.
 * @param amount_of_bytes Number of bytes to add.
 * @return error_t Error code indicating the success of the operation.
 */
error_t align_buffer(struct Buf_w_carriage_n_len *buf, size_t amount_of_bytes);

/**
 * @brief Writes a character to the buffer with alignment.
 *
 * @param byte_code Pointer to Buf_w_carriage_n_len structure representing buffer with position and length information.
 * @param value Character to be written.
 * @param alignment_space Number of alignment bytes.
 * @return error_t Error code indicating the success of the operation.
 */
error_t write_char_w_alignment(struct Buf_w_carriage_n_len *byte_code,
                               char value, size_t alignment_space);

/**
 * @brief Logs labels.
 *
 * Logs the labels along with their IDs and positions.
 *
 * @param labels_w_carriage Pointer to the structure containing labels.
 * @return Error code indicating the success or failure of the operation.
 */
error_t log_labels(struct Labels_w_carriage *labels_w_carriage);

/**
 * @brief Logs jumps.
 *
 * Logs the jumps along with their IDs and positions.
 *
 * @param jmp_poses_w_carriage Pointer to the structure containing jumps.
 * @return Error code indicating the success or failure of the operation.
 */
error_t log_jmps(struct JMP_poses_w_carriage *jmp_poses_w_carriage);

/**
 * @brief Arranges labels.
 *
 * Arranges labels in the bytecode based on their positions.
 *
 * @param cmds_process_result Result of command processing including bytecode, labels, jumps, etc.
 * @return Result of label arrangement including error code and updated bytecode.
 */
return_t arrange_labels(struct Cmds_process_result cmds_process_result);

/**
 * @brief Reduces buffer size.
 *
 * Reduces the size of the buffer to remove trailing zeros.
 *
 * @param buffer_w_info Buffer with carriage and length information.
 * @return Result of buffer size reduction including error code and updated buffer.
 */
return_t reduce_buffer_size(struct Buf_w_carriage_n_len buffer_w_info);

#endif
