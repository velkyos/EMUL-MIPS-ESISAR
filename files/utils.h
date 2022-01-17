#ifndef __UTILS_H__
#define __UTILS_H__

	#include <stdlib.h>
	#include <stdio.h>
	#include "memory.h"
	#include "registers.h"
	
	/* All Instruction OppCode */
	#define I_ADD 32
	#define I_ADDI 536870912
	#define I_AND 36
	#define I_BEQ 268435456
	#define I_BGTZ 469762048
	#define I_BLEZ 402653184
	#define I_BNE 335544320
	#define I_DIV 26
	#define I_J 134217728
	#define I_JAL 201326592
	#define I_JR 8
	#define I_LUI 1006632960
	#define I_LW 2348810240
	#define I_MFHI 16
	#define I_MFLO 18
	#define I_MULT 24
	#define I_NOP 0
	#define I_OR 37
	#define I_ROTR 2097154
	#define I_SLL 0
	#define I_SLT 42
	#define I_SRL 2
	#define I_SUB 34
	#define I_SW 2885681152
	#define I_SYSCALL 12
	#define I_XOR 38

	/* Specials constants */
	#define NBR_INSTRUCTION 26
	#define BUFFER_SIZE 128
	#define MAX_OPERATOR_SIZE 5
	#define MAX_OPERAND_SIZE 10
	#define	NO_ERROR 0
	#define ERROR_OPERATOR 1
	#define ERROR_OPERAND 2
	#define EMPTY_LINE 4

	

	/*	Compare two string, return 1 if equals 0 if not
	*	if onlyStart = 1, then string1 only need to start with string2 to return 1. */
	int string_compare(char * string1, char * string2, int onlyStart);

	/*   Concatenate s1 with s2 into output.
	*	Output is allocated into this function and must be free outside. */
	void string_concat(char** output, char *s1, char *s2);

	/*  Return the size of a string. */
	int string_size(char *s);

	/* Print the memory and used registers to the stream {p_file}. */
	void print_processor_to_stream(FILE *p_file,MemoryBlocks *p_memory, Registers *p_registers);

	/* Print errors */
	void handle_errors(int errors, int lineNbr);



#endif