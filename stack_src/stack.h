#ifndef STACK_H
#define STACK_H

#define DEBUG
#define CANARY_PROTECTION
#define HASH_PROTECTION
#define LOGGING

#ifdef LOGGING
	#define LOG(stk) fprintf((stk)->log_file ,"\n%s LOG:\n", __func__)
#endif

#ifdef DEBUG
	#define STACK_DUMP(stk, error_code) stack_dump(stk, #stk, error_code);
#endif

#ifdef DEBUG
#define STACK_PUSH(stk, push_value) stack_push((stk), (push_value), __FILE__,\
												__func__, __LINE__);
#define STACK_POP(stk) stack_pop((stk), __FILE__, __func__, __LINE__);
#define STACK_CTOR(stk, starter_capacity) stack_ctor(  (stk), starter_capacity,\
														__FILE__, __LINE__, __func__,\
														#stk "_log");
#endif

#ifdef DEBUG

	#define DUMP_W_COND(cond, ... )\
	if(cond)\
	{\
		fprintf(log_file, __VA_ARGS__);\
	}\
	else\
	{\
		fprintf(log_file, "FAILED CONDITION: ");\
		fprintf(log_file, #cond);\
		fprintf(log_file, "\n");\
	}

#endif

//#define ELEM_T_SPECIFIER #%d ???????????????????????????????????????????????????????
// stack_config.h

typedef double elem_t;

#ifdef CANARY_PROTECTION
	typedef unsigned long long canary_t;
	const canary_t canary_value = 0xBADC0FFEE;
	const canary_t buf_canary_value = 0xBAD;
#endif

const size_t stack_reformation_coeff = 2;
const size_t stack_expantion_size_from_zero = 5;
const elem_t DATA_POIZON = 666.666666;

enum Err_ID
{
	ALL_GOOD 				 = 0,
	SIZE_ZERO 				 = 1 << 0,
	SIZE_IS_GREATER			 = 1 << 1,
	DATA_NULL_PTR		 	 = 1 << 2,

	#ifdef DEBUG
		FILE_NAME_NULL_PTR	 = 1 << 3,
		FUNC_NAME_NULL_PTR	 = 1 << 4,
	#endif

	#ifdef HASH_PROTECTION
		DATA_HASH_AINT_RIGHT = 1 << 5,
		HASH_AINT_RIGHT		 = 1 << 12,
	#endif

	#ifdef CANARY_PROTECTION
		L_CANARY_DIED		 = 1 << 6,
		R_CANARY_DIED		 = 1 << 7,
		L_BUF_CANARY_DIED	 = 1 << 8,
		R_BUF_CANARY_DIED	 = 1 << 9,
	#endif

	STK_NULL_PTR 			 = 1 << 10,
	NOT_ENOUGH_MEM			 = 1 << 11,
	UNABLE_TO_OPEN_FILE  	 = 1 << 13,
	DATA_POIZON_VALUE		 = 1 << 14,
};


struct Stack_pop_result
{
	enum Err_ID error_code;
	elem_t deleted_element;
};

struct Stack
{
	#ifdef CANARY_PROTECTION
		canary_t left_canary;
	#endif

	#ifdef CANARY_PROTECTION
		canary_t *buf_left_canary;
	#endif
	elem_t *data;
	#ifdef CANARY_PROTECTION
		canary_t *buf_right_canary;
	#endif

	size_t size;
	size_t capacity;

	#ifdef DEBUG
		const char *file_name;
		const char *func_name;
		size_t line;
	#endif

	FILE *log_file;

	#ifdef HASH_PROTECTION
		size_t hash_check_value;
		size_t hash_data_check_value;
	#endif

	#ifdef CANARY_PROTECTION
		canary_t right_canary;
	#endif
};

enum Err_ID stack_ctor( Stack *stk, size_t starter_capacity
					#ifdef DEBUG
						,const char *file_name,
						size_t line, const char *func_name, const char *stk_name
					#endif
					  );
enum Err_ID stack_push( Stack *stk, elem_t value
					#ifdef DEBUG
						,const char *file_name, const char *func_name, size_t line
					#endif
					  );
struct Stack_pop_result stack_pop(  Stack *stk
								#ifdef DEBUG
									,const char *file_name,
									const char *func_name, size_t line
								#endif
								 );
enum Err_ID stack_dtor(Stack *stk);
enum Err_ID stack_verifier(const Stack *stk);
enum Err_ID stack_buf_realloc(Stack *stk);
void set_data_n_canary_ptrs(Stack *stk);

#ifdef HASH_PROTECTION
	size_t hash_count(void *stk, size_t hash_object_size);
	enum Err_ID hash_check_n_count(Stack *stk);
	void stack_hash_count(Stack *stk);
#endif


void print_data_elems(const Stack *stk, FILE *log_file);


#ifdef DEBUG
	#ifdef CANARY_PROTECTION
		bool unit_test_1(Stack *stk);
	#endif
#endif

#ifdef DEBUG
	void stack_dump(const Stack *stk, const char *stk_name, enum Err_ID error_code);
	void update_stack_invocation_position( Stack *stk,
								const char *file_name,
								size_t line,
								const char *func_name);
#endif

#endif
