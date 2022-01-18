#include "memory.h"

void write_to_memory(MemoryBlocks *p_memory, int address, char byte){
	MemoryBlock *p_new = NULL;
	MemoryBlock *p_current = *p_memory;
	MemoryBlock *p_last = NULL;



	/* If the memory is not Empty */
	if ( p_current != NULL)
	{
		while( p_current->p_next != NULL && p_current->address < address){
			p_last = p_current;
			p_current = p_current->p_next;
		}

		/* Modify an existing memory block */
		if ( p_current->address == address)
		{
			p_current->byte = byte;
		}else {

			p_new = malloc(sizeof(MemoryBlock));
			p_new->address = address;
			p_new->byte = byte;
			p_new->p_next = p_current;

			if (p_last != NULL)
			{
				p_last->p_next = p_new;
			}
			else{
				*p_memory = p_new;
			}
			
		}
	}
	else{
		p_new = malloc(sizeof(MemoryBlock));
		p_new->address = address;
		p_new->byte = byte;
		p_new->p_next = NULL;
		*p_memory = p_new;
	}

}


char read_from_memory(MemoryBlocks *p_memory, int address){
	MemoryBlock *p_current = NULL;

	char byte = rand() % 256; /* The default value if this memory cell is unknown. */

	if ( *p_memory != NULL)
	{
		p_current = *p_memory;
		while( p_current->address != address && p_current->p_next != NULL){
			p_current = p_current->p_next;
		}

		/* We find the block */
		if (p_current->address == address)
		{
			byte = p_current->byte;
		}
	}
	return byte;
}


void free_memory(MemoryBlocks *p_memory,int debug){
	MemoryBlock *p_current = NULL;
	MemoryBlock *p_next = NULL;
	int count = 0;

	printf("Memory Purge is in progress ...\n");

	/* If the memory is not already empty */
	if (*p_memory != NULL)
	{
		p_next = *p_memory;
		while(p_next != NULL){
			p_current = p_next;
			p_next = p_current->p_next;
			p_current->p_next = NULL;
			free(p_current);
			if (debug)
			{
				printf("The block at 0x%.8X was released\n",p_current->address);
			}
			count++;

		}
		*p_memory = NULL;
	}
	printf("Memory Purge is complete ! (%d elements)\n", count);
}