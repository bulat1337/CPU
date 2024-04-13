#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "stack.h"
#include "stack_additional.h"
#include "secondary.h"

enum Err_ID stack_ctor( Stack *stk, size_t starter_capacity
					#ifdef DEBUG
						,const char *file_name,
						size_t line, const char *func_name, const char *stk_name
					#endif
					  )
{
	enum Err_ID error_code = ALL_GOOD;

	#ifdef LOGGING
		#ifdef DEBUG
			stk->log_file = fopen(stk_name, "w");
		#else
			stk->log_file = fopen("logfile", "w");
		#endif
	#else
		stk->log_file = stdout;
	#endif

	if(!stk)
	{
		#ifdef DEBUG
			fprintf(stk->log_file, "Stack address is NULL\n");
		#endif

		return STK_NULL_PTR;
	}

	#ifdef CANARY_PROTECTION
		stk->left_canary = canary_value;
	#endif
	stk->capacity = starter_capacity;

	size_t data_size = sizeof(elem_t) * stk->capacity;
	#ifdef CANARY_PROTECTION
		size_t alignment_space = sizeof(canary_t) - data_size % sizeof(canary_t);
		stk->data = (elem_t *)calloc(1, data_size + 2 * sizeof(canary_t) + alignment_space);
	#else
		stk->data = (elem_t *)calloc(1, data_size);
	#endif
	if(stk->data == NULL)
	{
		#ifdef DEBUG
			fprintf(stk->log_file, "Not enough memory for allocation\n");
		#endif

		return	NOT_ENOUGH_MEM;
	}
	#ifdef CANARY_PROTECTION
		set_data_n_canary_ptrs(stk);
	#endif

	stk->size = 0;

	if(stk->log_file == NULL)
	{
		return UNABLE_TO_OPEN_FILE;
	}

	#ifdef DEBUG
		update_stack_invocation_position(stk, file_name, line, func_name);
	#endif

	#ifdef CANARY_PROTECTION
		stk->right_canary = canary_value;
	#endif

	#ifdef HASH_PROTECTION
		stack_hash_count(stk);
	#endif

	#ifdef LOGGING
		fprintf(stk->log_file, "*Stack has been created*\n");
	#endif


	#ifdef DEBUG
		STACK_DUMP(stk, error_code);
	#else
		#ifdef LOGGING
			print_data_elems(stk, log_file);
		#endif
	#endif


	return	ALL_GOOD;
}

enum Err_ID stack_push( Stack *stk, elem_t value
					#ifdef DEBUG
						,const char *file_name, const char *func_name, size_t line
					#endif
					  )
{
	#ifdef LOGGING
		LOG(stk);
	#endif

	#ifdef HASH_PROTECTION
		hash_check_n_count(stk);
	#endif

	enum Err_ID error_code = ALL_GOOD;

	#ifdef DEBUG
		update_stack_invocation_position(stk, file_name, line, func_name);
	#endif

	if((error_code = stack_verifier(stk)) != ALL_GOOD)
	{
		#ifdef DEBUG
			STACK_DUMP(stk, error_code);
		#endif

		return error_code;
	}

	if(stk->size >= stk->capacity)
	{
		if(stk->capacity == 0)
		{
			stk->capacity += stack_expantion_size_from_zero;
		}
		else
		{
			stk->capacity *= stack_reformation_coeff;
		}

		stack_buf_realloc(stk);

		#ifdef LOGGING
			fprintf(stk->log_file, "new capacity: expanded\n");
		#endif
	}

	stk->data[stk->size] = value;
	stk->size++;

	#ifdef HASH_PROTECTION
		stack_hash_count(stk);
	#endif

	#ifdef DEBUG
		STACK_DUMP(stk, error_code);
	#else
		#ifdef LOGGING
			print_data_elems(stk, log_file);
		#endif
	#endif

	return error_code;
}

struct Stack_pop_result stack_pop(  Stack *stk
								#ifdef DEBUG
									,const char *file_name,
									const char *func_name, size_t line
								#endif
								 )
{
	#ifdef LOGGING
		LOG(stk);
	#endif

	#ifdef HASH_PROTECTION
		hash_check_n_count(stk);
	#endif

	struct Stack_pop_result result = {};

	#ifdef DEBUG
		update_stack_invocation_position(stk, file_name, line, func_name);
	#endif

	if((result.error_code = stack_verifier(stk)) != ALL_GOOD)
	{
		#ifdef DEBUG
			STACK_DUMP(stk, result.error_code);
		#endif

		return result;
	}

	if(stk->size == 0)
	{
		result.error_code = SIZE_ZERO;

		#ifdef DEBUG
			fprintf(stk->log_file, "Stack_pop failed: size == 0\n");
		#endif

		return result;
	}

	(stk->size)--;
	result.deleted_element = (stk->data)[stk->size];

	if(stk->size <= stk->capacity / (stack_reformation_coeff * stack_reformation_coeff))
	{
		#ifdef LOGGING
			fprintf(stk->log_file, "Here is why capacity is going to be shrinked:\n");
			fprintf(stk->log_file, "\t size[%lu] <= (capacity / 4)[%lu]\n",
									stk->size, stk->capacity /
									(stack_reformation_coeff * stack_reformation_coeff));
		#endif
		stk->capacity /= stack_reformation_coeff;

		stack_buf_realloc(stk);
		#ifdef LOGGING
			fprintf(stk->log_file, "new capacity: shrinked\n");
		#endif
	}

	#ifdef HASH_PROTECTION
		stack_hash_count(stk);
	#endif

	#ifdef DEBUG
		STACK_DUMP(stk, result.error_code);
	#else
		#ifdef LOGGING
			print_data_elems(stk, log_file);
		#endif
	#endif

	return result;
}

enum Err_ID stack_dtor(Stack *stk)
{
	#ifdef LOGGING
		LOG(stk);
	#endif

	#ifdef HASH_PROTECTION
		hash_check_n_count(stk);
	#endif

	enum Err_ID error_code = ALL_GOOD;

	#ifdef LOGGING
		FILE *temp_log_file_ptr = stk->log_file;
	#endif

	if((error_code = stack_verifier(stk)) != ALL_GOOD)
	{
		#ifdef DEBUG
			STACK_DUMP(stk, error_code);
		#endif

		return error_code;
	}

	for(size_t data_ID = 0; data_ID < stk->capacity; data_ID++)
	{
		stk->data[data_ID] = DATA_POIZON;
	}
	#ifdef CANARY_PROTECTION
		free(stk->buf_left_canary);
		stk->buf_left_canary = NULL;
		stk->left_canary = 666;
		stk->right_canary = 666;
		stk->buf_right_canary = NULL;
	#endif
	stk->data = NULL;

	#ifdef HASH_PROTECTION
		stk->hash_check_value = 666;
		stk->hash_data_check_value = 666;
	#endif

	stk->log_file = NULL;

	#ifdef DEBUG
		stk->line = 666;
		stk->file_name = NULL;
		stk->func_name = NULL;
	#endif

	stk->size = 0;
	stk->capacity = 0;
	stk = NULL;

	#ifdef LOGGING
		fprintf(temp_log_file_ptr, "\n*Stack has been destroyed*\n");
	#endif

	return error_code;
}

enum Err_ID stack_verifier(const Stack *stk)
{
	int error_ID = 0;

	#define CHECK_CONDITION(cond, ERROR_TYPE)					\
	if((cond))													\
	{															\
		(error_ID) |= (ERROR_TYPE);								\
	}															\

	CHECK_CONDITION(stk->size < 0, SIZE_ZERO);
	CHECK_CONDITION(stk->size > stk->capacity, SIZE_IS_GREATER);
	if(stk->data != NULL)
	{
		for(size_t data_ID = 0; data_ID < stk->capacity; data_ID++)
		{
			CHECK_CONDITION(!cmp_double((stk->data)[data_ID], DATA_POIZON), DATA_POIZON_VALUE);
		}
	}
	else
	{
		error_ID |= DATA_NULL_PTR;
	}

	#ifdef DEBUG
		CHECK_CONDITION(stk->file_name == NULL, FILE_NAME_NULL_PTR);
		CHECK_CONDITION(stk->func_name == NULL, FUNC_NAME_NULL_PTR);
	#endif

	#ifdef CANARY_PROTECTION
		CHECK_CONDITION(stk->left_canary != canary_value, L_CANARY_DIED);
		CHECK_CONDITION(stk->right_canary != canary_value, R_CANARY_DIED);
		CHECK_CONDITION(*(stk->buf_left_canary) != buf_canary_value, L_BUF_CANARY_DIED);
		CHECK_CONDITION(*(stk->buf_right_canary) != buf_canary_value, R_BUF_CANARY_DIED);
	#endif

	#undef CHECK_CONDITION

	return (enum Err_ID)error_ID;
}

#ifdef HASH_PROTECTION

size_t hash_count(void *object, size_t hash_object_size)
{
	size_t hash_check_value = 0;
	for(size_t obj_elem_ID = 0; obj_elem_ID < hash_object_size; obj_elem_ID++)
	{
		hash_check_value += (size_t) *((char*)object + obj_elem_ID);
	}

	return hash_check_value;
}

void stack_hash_count(Stack *stk)
{
	stk->hash_check_value = 0;
	stk->hash_data_check_value = 0;

	size_t new_hash_check_value = hash_count(stk, sizeof(*stk));
	size_t new_hash_data_check_value = hash_count(stk->data, stk->capacity);

	stk->hash_check_value = new_hash_check_value;
	stk->hash_data_check_value = new_hash_data_check_value;
}

enum Err_ID hash_check_n_count(Stack *stk)
{
	#ifdef LOGGING
		LOG(stk);
	#endif

	enum Err_ID error_code = ALL_GOOD;
	size_t temp_hash_check_value = stk->hash_check_value;
	size_t temp_hash_data_check_value = stk->hash_data_check_value;

	stk->hash_check_value = 0;
	stk->hash_data_check_value = 0;

	size_t new_hash_check_value = hash_count(stk, sizeof(*stk));
	stk->hash_check_value = new_hash_check_value;
	stk->hash_data_check_value = hash_count(stk->data, stk->capacity);

	if((new_hash_check_value != temp_hash_check_value) ||
	   (stk->hash_data_check_value != temp_hash_data_check_value))
	{
		error_code = HASH_AINT_RIGHT;
		#ifdef DEBUG
			fprintf(stk->log_file, "ERROR:\n"
								   "new_hash_check_value == %lu\n"
								   "temp_hash_check_value == %lu\n"
								   "hash_data_check_value == %lu\n"
								   "temp_hash_data_check_value = %lu\n",
									new_hash_check_value, temp_hash_check_value,
									stk->hash_data_check_value, temp_hash_data_check_value);
			STACK_DUMP(stk, error_code);
		#endif
	}

	fprintf(stk->log_file, "new_hash_check_value[%lu] == temp_hash_check_value[%lu]\n"
						   "hash_data_check_value[%lu] == temp_hash_data_check_value[%lu]\n",
							new_hash_check_value, temp_hash_check_value,
							stk->hash_data_check_value, temp_hash_data_check_value);

	return error_code;
}
#endif

enum Err_ID stack_buf_realloc(Stack *stk)
{
	enum Err_ID error_code = ALL_GOOD;

	size_t data_size = sizeof(elem_t) * stk->capacity;
	#ifdef CANARY_PROTECTION
		size_t alignment_space = sizeof(canary_t) - data_size % sizeof(canary_t);
		stk->data =
			(elem_t *)realloc(stk->buf_left_canary,
			data_size + 2 * sizeof(canary_t) + alignment_space);
	#else
		stk->data = (elem_t *)realloc(stk->data, data_size);
	#endif

	if(stk->data == NULL)
	{
		return NOT_ENOUGH_MEM;
	}

	#ifdef CANARY_PROTECTION
		set_data_n_canary_ptrs(stk);
	#endif

	return error_code;
}

#ifdef CANARY_PROTECTION
void set_data_n_canary_ptrs(Stack *stk)
{
	size_t data_size = sizeof(elem_t) * stk->capacity;
	size_t alignment_space = sizeof(canary_t) - data_size % sizeof(canary_t);
	size_t additional_mem = sizeof(canary_t) + alignment_space;

	stk->buf_left_canary = (canary_t *)stk->data;
	stk->buf_right_canary = (canary_t *)((char *)(stk->data) + additional_mem + data_size);
	stk->data = stk->data + (sizeof(canary_t) / sizeof(elem_t));
	*(stk->buf_left_canary) = buf_canary_value;
	*(stk->buf_right_canary) = buf_canary_value;
}
#endif

#ifdef DEBUG
void stack_dump(const Stack *stk, const char *stk_name, enum Err_ID error_code)
{
	FILE *log_file = stk->log_file;
	if(log_file == NULL)
	{
		log_file = stderr;
		fprintf(log_file, "stk->log_file == NULL\n");
	}

	if(error_code != ALL_GOOD)
	{
		fprintf(log_file, "ERROR:\n");

		#define ERROR_CHECK(err_type)\
		if(error_code & err_type)\
		{\
			fprintf(log_file, "-");\
			fprintf(log_file, #err_type);\
			fprintf(log_file, "\n");\
		}\

		// switch(SIZE_ZERO)
		// {
		// 	case (SIZE_ZERO):
		// 		ERROR_CHECK(SIZE_ZERO);
		// 	case (SIZE_IS_GREATER):
		// 		ERROR_CHECK(SIZE_IS_GREATER);
		// }

		ERROR_CHECK(SIZE_ZERO);
		ERROR_CHECK(SIZE_IS_GREATER);
		ERROR_CHECK(DATA_NULL_PTR);
		ERROR_CHECK(DATA_POIZON_VALUE);
		ERROR_CHECK(FILE_NAME_NULL_PTR);
		ERROR_CHECK(FUNC_NAME_NULL_PTR);
		#ifdef HASH_PROTECTION
			ERROR_CHECK(DATA_HASH_AINT_RIGHT);
			ERROR_CHECK(HASH_AINT_RIGHT);
		#endif
		#ifdef CANARY_PROTECTION
			ERROR_CHECK(L_CANARY_DIED);
			ERROR_CHECK(R_CANARY_DIED);
			ERROR_CHECK(L_BUF_CANARY_DIED);
			ERROR_CHECK(R_BUF_CANARY_DIED);
		#endif
		ERROR_CHECK(STK_NULL_PTR);
		ERROR_CHECK(NOT_ENOUGH_MEM);
		ERROR_CHECK(UNABLE_TO_OPEN_FILE);

		#undef ERROR_CHECK
	}

	if(stk == NULL)
	{
		FILE *fatal_error = fopen("FATAL_ERROR_LOG.txt", "w");
		if(fatal_error == NULL)
		{
			fatal_error = stderr;
		}

		fprintf(fatal_error, "Stack[%p] address is NULL\n", stk); //printf для stk->log_file
	}
	else
	{
		DUMP_W_COND(stk_name != NULL, "%s[%p] dump:\n", stk_name, (stk));
		#ifdef CANARY_PROTECTION
		DUMP_W_COND(&(stk->left_canary) != NULL, "left_canary[%p] = %llX\n", &(stk->left_canary),
																			   stk->left_canary);
		DUMP_W_COND(&(stk->right_canary) != NULL, "right_canary[%p] = %llX\n", &(stk->right_canary),
																 				 stk->right_canary);
		DUMP_W_COND(stk->buf_left_canary != NULL, "buf_left_canary[%p] = %llX\n",
												   stk->buf_left_canary, *(stk->buf_left_canary));
		DUMP_W_COND(stk->buf_right_canary != NULL, "buf_right_canary[%p] = %llX\n",
													stk->buf_right_canary, *(stk->buf_left_canary));
		#endif
		DUMP_W_COND(&(stk->size)     != NULL, "size = %lu\n",     stk->size);
		DUMP_W_COND(&(stk->capacity) != NULL, "capacity = %lu\n", stk->capacity);
		DUMP_W_COND(stk->func_name   != NULL, "function: %s\n",   stk->func_name);
		DUMP_W_COND(&(stk->line)     != NULL, "line: %lu\n",      stk->line);
		DUMP_W_COND(stk->file_name   != NULL, "file: %s\n",       stk->file_name);
		fprintf(log_file, "data addres = %p\n", stk->data);
		if((&(stk->capacity) != NULL) && (stk->data != NULL))
		{
			print_data_elems(stk, log_file);
		}
		else
		{
			fprintf(log_file, "(&(stk->capacity) == NULL) && stk->data == NULL\n");
		}

		fprintf(log_file, "error byte code: ");
		show_bits(error_code, log_file);
	}
}
#endif

#ifdef DEBUG
void update_stack_invocation_position( Stack *stk, const char *file_name,
							size_t line, const char *func_name)
{
	stk->file_name = file_name;
	stk->func_name = func_name;
	stk->line = line;
}
#endif

#ifdef DEBUG
#ifdef CANARY_PROTECTION
bool unit_test_1(Stack *stk) // принимать стк не нужно
{
	STACK_CTOR(stk, 0);
	STACK_PUSH(stk, 666);
	STACK_PUSH(stk, 666);
	STACK_POP(stk);
	// константы для референсных значений
	if((*(stk->buf_left_canary) == buf_canary_value) &&
	   (*(stk->buf_right_canary) == buf_canary_value) &&
	   (stk->capacity == 2) &&
	   (!cmp_double((stk->data)[0], 666)) &&
	   (stk->left_canary == canary_value) &&
	   (stk->right_canary == canary_value) &&
	   (stk->size == 0))
	{
		stack_dtor(stk);
		return 1;
	}
	else
	{
		stack_dtor(stk);
		return 0;
	}
}
#endif
#endif


void print_data_elems(const Stack *stk, FILE *log_file)
{
	for(size_t stk_ID = 0; stk_ID < stk->capacity; stk_ID++)
	{
		fprintf(log_file, "[%lu] = %lf\n", stk_ID, stk->data[stk_ID]);
	}
}

