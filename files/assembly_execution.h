#ifndef __EXECUTION_ASSEMBLY_H__
	#define __EXECUTION_ASSEMBLY_H__

	#include "assembly_compilation.h"

	/* Run the program in interactive mode.*/
	void run_in_interactive();

	/* Compile the file named {p_pathIn} into the file named {p_pathHexOut}, execute it and save the memory and registers into {p_pathFinOut}.
	* If {p_pathFinOut} is NULL then the program is launch into the step-by-step mode.*/
  	void run_a_file(char *p_pathIn,char *p_pathHexOut,char *p_pathFinOut);

#endif