#ifndef COMMANDS_H
#define COMMANDS_H

// enum Byte_code_type
// {
// 	CMD_W_8_BYTE_ARG = 1,
// 	CMD_W_4_BYTE_ARG = 2,
// 	CMD_W_NO_ARG     = 3,
// 	CMD_W_LABEL_ARG  = 4,
// 	LABEL            = 5,
// };

enum Command
{
	VOID = 0,
	PUSH = 1,
	ADD  = 2,
	SUB  = 3,
	MUL  = 4,
	DIV  = 5,
	OUT  = 6,
	IN   = 7,
	POP  = 8,
	JMP  = 9,
	JA   = 10,
	JB   = 11,
	JAE  = 12,
	JBE  = 13,
	JE   = 14,
	JNE  = 15,
	CALL = 16,
	RET  = 17,
	DRAW = 18,
	SQRT = 19,
	LN   = 20,
	SIN  = 21,
	COS  = 22,
	POW  = 23,
	HLT  = -1,
};

#endif
