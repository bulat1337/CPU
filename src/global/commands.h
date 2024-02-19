#ifndef COMMANDS_H
#define COMMANDS_H

enum Command
{
	ZERO = 0, 
	PUSH = 1, 
	ADD = 2,
	SUB = 3,
	MUL = 4,
	DIV = 5,
	OUT = 6,
	IN  = 7,   
	POP = 8,   
	JMP = 9,
	JA = 10,
	JB = 11,
	JAE = 12,
	JBE = 13,
	JE = 14,
	JNE = 15,
	CALL = 16,
	RET = 17,
	HLT = -1,
};

#endif
