#ifndef _ASSEMBLY_COMPILATION_H_
#define _ASSEMBLY_COMPILATION_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include "utils.h"
	

	/* Compile the file named {p_pathIn} into the file named {p_pathOut} */
	int compile_file(char *p_pathIn, char *p_pathOut);

	/* Load the already compiled file named {p_pathIn} into the list of int {p_program}. */
	void load_compiled_file(char *p_pathIn, int *p_program, int needDelete);

	/*	Compile {p_line} and store the result in {p_instruction} and return the corresponding errorCode */
	int compile_line(char *p_line, int *p_instruction);

	/* Decompile the instruction given by {operator} and {p_operands} and stored the string into {s_output}. */
	void decompile_instruction(char ** s_output, int operator,int* p_operands);

	/* Write the content of the memory and registers into the file named {p_path} with the help of  'print_registers_to_file'. */
	void write_final_file(char *p_path, Registers *p_registers);
	
	
	

#endif 
