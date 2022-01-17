#ifndef __MEMORY_H__
#define __MEMORY_H__

	#include <stdio.h>
	#include <stdlib.h>
	
	/* Structs */

	typedef struct Memory_struct MemoryBlock;
	struct Memory_struct{ 
		unsigned char byte;  	/* The value of the block */
		unsigned int address;	/* The address of the block */
		MemoryBlock *p_next; 
	};

	typedef MemoryBlock* MemoryBlocks;

	/* Functions prototypes */

	/* Write {byte} at {address}. */
	void write_to_memory(MemoryBlocks *p_memory, int address, char byte);

	/* Return the byte stored at {address} or an unknown value. */
	char read_from_memory(MemoryBlocks *p_memory, int address);

	/* Free all of the memory. */
	void free_memory(MemoryBlocks *p_memory,int debug);

#endif