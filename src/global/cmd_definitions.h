DEF_CMD
(
	push, PUSH, CMD_W_8_BYTE_ARG,

	if(*(int *)(CURRENT_BYTE_CODE) & RAM_IDENTIFIER_MASK)
	{
		if(*(int *)(CURRENT_BYTE_CODE) & CONSTANT_IDENTIFIER_MASK)
		{
			RAM_address = *(unsigned int *)(CURRENT_BYTE_CODE + sizeof(int));

			value = vm.rand_access_mem.user_RAM[RAM_address];

			STACK_PUSH(&(vm.user_stack), value);
		}
		else if(*(int *)(CURRENT_BYTE_CODE) & REGISTER_IDENTIFIER_MASK)
		{
			reg_type = *(unsigned int *)(CURRENT_BYTE_CODE + sizeof(int));

			RAM_address = (unsigned int)vm.registers[reg_type];

			value = vm.rand_access_mem.user_RAM[RAM_address];

			STACK_PUSH(&(vm.user_stack), value);
		}
		else
		{
			return INVALID_RAM_MODE;
		}
	}
	else if(*(int *)(CURRENT_BYTE_CODE) & CONSTANT_IDENTIFIER_MASK)
	{
		MOVE_CARRIAGE;

		STACK_PUSH(&(vm.user_stack), *(elem_t *)CURRENT_BYTE_CODE);
	}
	else if(*(int *)(CURRENT_BYTE_CODE) & REGISTER_IDENTIFIER_MASK)
	{
		reg_type = *(unsigned int *)(CURRENT_BYTE_CODE + sizeof(int));
		STACK_PUSH(&(vm.user_stack), vm.registers[reg_type]);
	}

	MOVE_CARRIAGE;
)

DEF_CMD
(
	pop, POP, CMD_W_4_BYTE_ARG,

	if(*(int *)(CURRENT_BYTE_CODE) & RAM_IDENTIFIER_MASK)
	{
		RAM_address = *(unsigned int *)(CURRENT_BYTE_CODE + sizeof(int));

		vm.rand_access_mem.user_RAM[RAM_address] =
			STACK_POP(&(vm.user_stack)).deleted_element;
	}
	else if(*(int *)(CURRENT_BYTE_CODE) & REGISTER_IDENTIFIER_MASK)
	{
		reg_type = *(unsigned int *)(CURRENT_BYTE_CODE + sizeof(int));

		vm.registers[reg_type] =
			STACK_POP(&(vm.user_stack)).deleted_element;
	}


	MOVE_CARRIAGE;
)

DEF_CMD
(
	in, IN, CMD_W_NO_ARG,

	printf("Please enter value: ");

	scanf("%lf", &user_entered_value);
	clear_buffer();

	STACK_PUSH(&(vm.user_stack), user_entered_value);

	MOVE_CARRIAGE;
)

DEF_CMD
(
	add, ADD, CMD_W_NO_ARG,

	value_B = STACK_POP(&(vm.user_stack)).deleted_element;
	value_A = STACK_POP(&(vm.user_stack)).deleted_element;

	STACK_PUSH(&(vm.user_stack), value_A + value_B);

	MOVE_CARRIAGE;
)

DEF_CMD
(
	sub, SUB, CMD_W_NO_ARG,

	value_B = STACK_POP(&(vm.user_stack)).deleted_element;
	value_A = STACK_POP(&(vm.user_stack)).deleted_element;

	STACK_PUSH(&(vm.user_stack), value_A - value_B);

	MOVE_CARRIAGE;
)

DEF_CMD
(
	mul, MUL, CMD_W_NO_ARG,

	value_B = STACK_POP(&(vm.user_stack)).deleted_element;
	value_A = STACK_POP(&(vm.user_stack)).deleted_element;

	STACK_PUSH(&(vm.user_stack), value_A * value_B);

	MOVE_CARRIAGE;
)

DEF_CMD
(
	div, DIV, CMD_W_NO_ARG,

	value_B = STACK_POP(&(vm.user_stack)).deleted_element;
	value_A = STACK_POP(&(vm.user_stack)).deleted_element;

	STACK_PUSH(&(vm.user_stack), value_A / value_B);

	MOVE_CARRIAGE;
)

DEF_CMD
(
	out, OUT, CMD_W_NO_ARG,

	value = STACK_POP(&(vm.user_stack)).deleted_element;

	fprintf(output_file, "RESULT: %.3lf\n", value);

	MOVE_CARRIAGE;
)

DEF_CMD
(
	ret, RET, CMD_W_NO_ARG,

	byte_code_carriage = (size_t)STACK_POP(&(vm.ret_stack)).deleted_element;
)

DEF_CMD
(
	jmp, JMP, CMD_W_LABEL_ARG,

	UPDATE_BYTE_CODE_CARRIAGE;
)

DEF_CMD
(
	jae, JAE, CMD_W_LABEL_ARG,

	value_B = STACK_POP(&(vm.user_stack)).deleted_element;
	value_A = STACK_POP(&(vm.user_stack)).deleted_element;

	cmp_result = cmp_double(value_A, value_B);

	if(cmp_result == 1 || cmp_result == 0)
	{
		UPDATE_BYTE_CODE_CARRIAGE;
	}
	else
	{
		MOVE_CARRIAGE;
	}
)

DEF_CMD
(
	ja, JA, CMD_W_LABEL_ARG,

	value_B = STACK_POP(&(vm.user_stack)).deleted_element;
	value_A = STACK_POP(&(vm.user_stack)).deleted_element;

	cmp_result = cmp_double(value_A, value_B);

	if(cmp_result == 1)
	{
		UPDATE_BYTE_CODE_CARRIAGE;
	}
	else
	{
		MOVE_CARRIAGE;
	}
)

DEF_CMD
(
	jbe, JBE, CMD_W_LABEL_ARG,

	value_B = STACK_POP(&(vm.user_stack)).deleted_element;
	value_A = STACK_POP(&(vm.user_stack)).deleted_element;

	cmp_result = cmp_double(value_A, value_B);

	if(cmp_result == -1 || cmp_result == 0)
	{
		UPDATE_BYTE_CODE_CARRIAGE;
	}
	else
	{
		MOVE_CARRIAGE;
	}
)

DEF_CMD
(
	jb, JB, CMD_W_LABEL_ARG,

	value_B = STACK_POP(&(vm.user_stack)).deleted_element;
	value_A = STACK_POP(&(vm.user_stack)).deleted_element;

	cmp_result = cmp_double(value_A, value_B);

	if(cmp_result == -1)
	{
		UPDATE_BYTE_CODE_CARRIAGE;
	}
	else
	{
		MOVE_CARRIAGE;
	}
)

DEF_CMD
(
	je, JE, CMD_W_LABEL_ARG,

	value_B = STACK_POP(&(vm.user_stack)).deleted_element;
	value_A = STACK_POP(&(vm.user_stack)).deleted_element;

	cmp_result = cmp_double(value_A, value_B);

	if(cmp_result == 0)
	{
		UPDATE_BYTE_CODE_CARRIAGE;
	}
	else
	{
		MOVE_CARRIAGE;
	}
)

DEF_CMD
(
	jne, JNE, CMD_W_LABEL_ARG,

	value_B = STACK_POP(&(vm.user_stack)).deleted_element;
	value_A = STACK_POP(&(vm.user_stack)).deleted_element;

	cmp_result = cmp_double(value_A, value_B);

	if(cmp_result != 0)
	{
		UPDATE_BYTE_CODE_CARRIAGE;
	}
	else
	{
		MOVE_CARRIAGE;
	}
)

DEF_CMD
(
	call, CALL, CMD_W_LABEL_ARG,

	STACK_PUSH(&(vm.ret_stack), byte_code_carriage + sizeof(double));

	UPDATE_BYTE_CODE_CARRIAGE;
)

DEF_CMD
(
	hlt, HLT, CMD_W_NO_ARG,

	return SPU_ALL_GOOD;
)

DEF_CMD
(
	draw, DRAW, CMD_W_NO_ARG,

	for(size_t address = 0; address < USER_RAM_SIZE; address++)
	{
		fprintf(output_file, "%c ", (char)vm.rand_access_mem.user_RAM[address]);

		if((address + 1) % 20 == 0)
		{
			fprintf(output_file, "\n");
		}
	}

	MOVE_CARRIAGE;
)

DEF_CMD
(
	sqrt, SQRT, CMD_W_NO_ARG,

	value = STACK_POP(&(vm.user_stack)).deleted_element;

	STACK_PUSH(&(vm.user_stack), sqrt(value));

	MOVE_CARRIAGE;
)

DEF_CMD
(
	:, VOID, LABEL,

	;
)
