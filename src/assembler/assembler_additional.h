#ifndef ASSEMBLER_ADDITIONAL
#define ASSEMBLER_ADDITIONAL

#include "assembler.h"
#include "file_parse.h"

struct Label
{
	char  *name;
	size_t IP_pos;
};

struct Labels_w_carriage
{
	struct Label *labels;
	size_t carriage;
};

struct JMP_pos
{
	char const *name;
	int IP_pos;
};

struct JMP_poses_w_carriage
{
	struct JMP_pos *JMP_poses;
	size_t carriage;
};

struct Parse_human_code_result
{
	error_t error_code;
	char * *strings;
	size_t amount_of_lines;
	struct Buffer_w_info human_code_buffer;
};

struct Cmds_process_result
{
	error_t                     error_code;
	struct Buf_w_carriage_n_len buf_w_info;
	struct Labels_w_carriage    labels_w_carriage;
	struct JMP_poses_w_carriage jmp_poses_w_carriage;
};

const int    POISON_JMP_POS            = -1;
const char   IDENTIFIER_BYTE           =  1;
const int    CMD_TYPE_ALIGNMENT_VALUE  =  3;
const size_t ALIGN_TO_INT              =  sizeof(int) - sizeof(char);
const size_t ALIGN_TO_DOUBLE           =  sizeof(double) - sizeof(char);
const char * const MAIN_JMP_NAME       = "main";
const unsigned char SPACE_SKIP         = 1;
const unsigned char LETTER_SKIP        = 1;
const unsigned char SIX_BYTE_ALIGNMENT = 6;
const unsigned char ONE_BYTE_ALIGNMENT = 1;

struct Parse_human_code_result parse_human_code(const char *file_name);
struct Cmds_process_result cmds_process(char * *commands, size_t amount_of_lines);
error_t write_main_jmp(struct Buf_w_carriage_n_len *byte_code,
					   JMP_poses_w_carriage *jmp_poses_w_carriage);
error_t write_to_buf(struct Buf_w_carriage_n_len *byte_code,
					 const void *value, size_t size);
error_t align_buffer(struct Buf_w_carriage_n_len *buf, size_t amount_of_bytes);
error_t write_char_w_alignment(struct Buf_w_carriage_n_len *byte_code,
							   char value, size_t alignment_space);
error_t log_labels(struct Labels_w_carriage *labels_w_carriage);
error_t log_jmps(struct JMP_poses_w_carriage *jmp_poses_w_carriage);
return_t arrange_labels(struct Cmds_process_result cmds_process_result);
return_t reduce_buffer_size(struct Buf_w_carriage_n_len buffer_w_info);

#endif
