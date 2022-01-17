#ifndef __REGISTERS_H__
#define __REGISTERS_H__

	#include <stdio.h>
	
	#define REGISTER_SIZE 32
	#define R_PC 32
	#define R_HI 33
	#define R_LO 34

	typedef struct Registers_struct{ 
		int GPR[REGISTER_SIZE]; 		/* All 32 defaults Registers */
		int haveModif[REGISTER_SIZE];	/* Boolean list to tell for each register if it as been modified yet */
		int PC, HI, LO;					/* Special Register */
	} Registers;


	/* Functions prototypes */

	/* Initialize all registers modification status at 0*/
	void init_registers(Registers *p_registers);

	/* Write {value} to the register number {registerId} */
	void write_to_register(Registers *p_registers, int registerId, int value);

	/* Return the value of the register number {registerId} */
	int read_from_register(Registers *p_registers, int registerId);

	/* Increase the program counter by {value}. */
	void increase_pc(Registers *p_registers, int value);

	/* Return how many registers you have modified from the start of the program. */
	int count_modified_registers(Registers *p_registers);

	/* Return the number of next modified registers (after the {last} one). */
	int get_next_modified_register(Registers *p_registers, int last);

	/* Print all registers into a file. */
	void print_registers_to_file(FILE *p_file, Registers *p_registers);

#endif