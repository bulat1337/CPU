#include "SPU_additional.h"
#include "SPU.h"

error_t execute(const char *file_name)
{
	FILE *byte_code_file = fopen(file_name, "rb");

	if(byte_code_file == NULL)
	{
		log("Unable to open %s\n", file_name);

		return SPU_UNABLE_TO_OPEN_FILE;
	}

	size_t byte_code_size = get_file_length(byte_code_file);

	printf("%lu\n", byte_code_size);
	// struct Buf_w_carriage_n_len byte_code =
	// {
	// 	.carriage = 0,
	// 	.length =
	// };


	return SPU_ALL_GOOD;
}
