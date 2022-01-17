#include "utils.h"


int string_compare(char * s1, char * s2, int onlyStart){
	int i = 0;
	int isEqual = 1;

	// we run through the string until we find a difference or until the end of one of the two strings 
	while (isEqual && s1[i] != '\0' && s2[i] != '\0') {
		// if the character of the first string doesn't match with the second one, strings are not the same
		if (s1[i] != s2[i]) {
			isEqual = 0;
		}
		i++;
	}

	/* Detect if one string is not finish and onlystart is not check */
	if ( !( s1[i] == '\0' && s2[i] == '\0' ) && !onlyStart) {
		isEqual = 0;
	}

	return isEqual && s2[i] == '\0';
}

void string_concat(char** output, char *s1, char *s2){
	int size1 = string_size(s1);
	int size2 = string_size(s2);
	int i;

	*output = (char *)malloc(sizeof(char) * (size1 + size2 + 1));
	
	for (i = 0; i < size1; i++)
	{
		(*output)[i] = s1[i];
	}
	for (i = 0; i < size2; i++)
	{
		(*output)[size1 + i] = s2[i];
	}
	(*output)[size1 + size2] = '\0';
}

int string_size(char *s){
	int size = 0;
	while ( s[size] != '\0')
	{
		size++;
	}
	return size;
}

void handle_errors(int errors, int lineNbr){
	
	if ( errors != NO_ERROR && errors != EMPTY_LINE)
	{
		
		if ( (errors & ERROR_OPERAND) == ERROR_OPERAND)
		{
			if (lineNbr < 0) printf("Error detected on an Operand.\n");
			else printf("Error detected on an Operand in the line n° %d.\n", lineNbr);
		}
		if ( (errors & ERROR_OPERATOR) == ERROR_OPERATOR)
		{
			if (lineNbr < 0) printf("Error detected on the Operator.\n");
			else printf("Error detected on the Operator in the line n° %d.\n", lineNbr);
		}
	}
	
}

void print_processor_to_stream(FILE *p_file,MemoryBlocks *p_memory, Registers *p_registers){
	int registerAllreadyPrint = 0;
	int nextRegister = -1;
	int nbrModifiedRegisters = count_modified_registers(p_registers);
	int moreRegisters =  nbrModifiedRegisters > registerAllreadyPrint;

	int moreData = *p_memory != NULL;
	MemoryBlock *p_current = moreData ? *p_memory : NULL;

	int mustPrintOther = 0;

	fprintf( p_file, "-----------------------------------------------------------\n");
	fprintf( p_file, "| PC | 0x%.8X | | HI | 0x%.8X | | LO | 0x%.8X |\n", p_registers->PC, p_registers->HI, p_registers->LO);
	fprintf( p_file, "-----------------------------------------------------------\n");
	fprintf( p_file, "|        REGISTERS TABLE        |- -|    MEMORY  TABLE    |\n");
	fprintf( p_file, "--------------------------------|- -|----------------------\n");

	/* moreRegisters > 0 at the start because the register 'zero' is always shown */
	if ( moreData && moreRegisters) /* If the memory is not empty */
	{
		fprintf( p_file, "| N° |    Hexa    |   Decimal   |- -|   Address  | Valeur |\n");
		fprintf( p_file, "|-------------------------------|- -|---------------------|\n");
	}else if (moreRegisters){ /* If the memory is empty */
		fprintf( p_file, "| N° |    Hexa    |   Decimal   |- -|       (Empty)\n");
		fprintf( p_file, "|-------------------------------|- -|\n");
	}
	
	while ( moreRegisters && moreData )
	{


		nextRegister = get_next_modified_register(p_registers, nextRegister);
		fprintf( p_file, "| %02d | 0x%.8X | %.*d |- -| 0x%.8X |  0x%.2X  |\n", nextRegister, p_registers->GPR[nextRegister], 11, p_registers->GPR[nextRegister], p_current->address, p_current->byte);

		p_current = p_current->p_next;
		registerAllreadyPrint++;

		moreRegisters = nbrModifiedRegisters > registerAllreadyPrint;
		moreData = p_current != NULL;

		if ( !moreRegisters && !moreData) fprintf( p_file, "--------------------------------|- -|----------------------\n");
		else if ( !moreRegisters || !moreData)
		{
			mustPrintOther = 1;
		}

	}
	
	/* If we need to print more registers */
	while ( moreRegisters)
	{
		nextRegister = get_next_modified_register(p_registers, nextRegister);
		fprintf( p_file, "| %02d | 0x%.8X | %*d |- -|", nextRegister, p_registers->GPR[nextRegister], 11, p_registers->GPR[nextRegister]);

		/* Print the end of memory */
		if ( mustPrintOther)
		{
			fprintf( p_file, "----------------------\n");
			mustPrintOther = 0;
		}else{
			fprintf( p_file, "\n");
		}

		registerAllreadyPrint++;
		moreRegisters = nbrModifiedRegisters > registerAllreadyPrint;

		/* Print the end of register */
		if (!moreRegisters)
		{
			fprintf( p_file, "--------------------------------|- -|\n");
		}
	}

	/* If we need to print more data */
	while ( moreData)
	{
		/* Print the end of register */
		if ( mustPrintOther)
		{
			fprintf( p_file, "--------------------------------");
			mustPrintOther = 0;
		}else{
			fprintf( p_file, "                                ");
		}
		
		fprintf( p_file, "|- -| 0x%.8X |  0x%.2X  |\n", p_current->address, p_current->byte);

		p_current = p_current->p_next;
		moreData = p_current != NULL;

		/* Print the end of memory */
		if (!moreData)
		{
			fprintf( p_file, "                                |- -|----------------------\n");
		}
		
	}
	
	
}