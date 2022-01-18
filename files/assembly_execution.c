#include "assembly_execution.h"

/* Execute the instruction {instruction} and modified {p_memory} and {p_registers} */
void execute_instruction(int instruction,int needConfirm ,MemoryBlocks *p_memory, Registers *p_registers);

/* Get oppcode {p_operator} and operands {p_operands} from the instruction {instruction}. */
void get_instruction_values(int instruction, unsigned int* p_operator, int* p_operands);

void run_in_interative(){
	int current_pc;
	Registers *registers = malloc(sizeof(Registers));
	MemoryBlocks memory = NULL;

	char currentInstruction[BUFFER_SIZE];
	int currentInstructionInt; /* The instruction integer after being compiled*/
	int error;

	printf("Launch of the interactive mode ...\n");

	/* 1 - Initialization of the registers */
	init_registers(registers);

	while ( string_compare(currentInstruction, "EXIT",0) != 1)
	{
		
		current_pc = read_from_register(registers, R_PC);

		/* 2 - We ask for the instruction at this line (PC / 4)*/
		printf("\nInstruction n°%d : ", current_pc / 4);

		fgets( currentInstruction, BUFFER_SIZE, stdin);

		/* 3 - We compile the line and save the error code */
		error = compile_line(currentInstruction, &currentInstructionInt);

		handle_errors(error, -1);

		if (error == 0)
		{
			/* 4 - Execution of the instruction */
			execute_instruction( currentInstructionInt,0, &memory, registers);

			/* 4.5 - Show the value of the processor into the console */
			print_processor_to_stream(stdout, &memory, registers);
		}
	}
	
	/* 5 - Free all variables */
	free_memory(&memory,0);
	free(registers);

	printf("End of the program !\n");
}

void run_a_file(char *p_pathIn,char *p_pathHexOut,char *p_pathFinOut){
	int *p_instructions = NULL;
	int instructionNbr;
	int current_pc;
	int needConfirm = p_pathFinOut == NULL;

	Registers *registers = malloc(sizeof(Registers));
	MemoryBlocks memory = NULL;

	/* 1 - Compile the file */
	instructionNbr = compile_file(p_pathIn,p_pathHexOut);
	
	p_instructions = malloc(sizeof(int) * instructionNbr);

	/* 2 - Load all instruction in 'memory' */
	load_compiled_file(p_pathHexOut, p_instructions, needConfirm);
	
	printf("Launch of the non-interactive mode ...\n");

	/* 3 - Initialization of the registers */
	init_registers(registers);

	current_pc = read_from_register(registers, R_PC);

	while( (current_pc / 4) < instructionNbr){ 

		/* 4 - Execute the instruction with or without confirmation */
		execute_instruction(p_instructions[ current_pc / 4 ], needConfirm, &memory, registers);

		if ( needConfirm )
		{	
			/* 4.5 - We only show the value of registers and memory if we need confirmation */
			print_processor_to_stream(stdout, &memory, registers);
		}
		

		current_pc = read_from_register(registers, R_PC);
	}

	if ( !needConfirm )
	{
		/* We write into the '.state' file only if we don't have confirmation */
		write_final_file(p_pathFinOut, registers);
	}
	
	/* 5 - Free all variables */
	free(p_instructions);
	free_memory(&memory,0);
	free(registers);
	printf("End of the program !\n");
}

void execute_instruction(int instruction,int needConfirm, MemoryBlocks *p_memory, Registers *p_registers){
	unsigned int operator;
	int operands[3];
	int i;
	int temp1, temp2, temp3;
	long tempLong;
	int PC_increased = 0;
	char *instructionText = NULL;

	/* 1 - Get oppcode and operands*/
	get_instruction_values(instruction, &operator, operands);

	/* 2 - Show the current instruction */
	decompile_instruction(&instructionText, operator, operands);
	printf( "\nExecute : %s ( 0x%.8X )" , instructionText, instruction);
	free(instructionText);

	/* 2.5 - Ask confirmation if needed */
	if ( needConfirm)
	{
		int confirm = 0;
		printf(" ?");
		while( confirm != '\r' && confirm != '\n'){
			confirm = getchar();
		}
	}
	else{
		printf("\n");
	}
	
	/* 3 - Big if for each instruction */
	if (operator == I_ADD) {
		temp1 = read_from_register(p_registers, operands[1]);
		temp2 = read_from_register(p_registers, operands[0]);

		write_to_register(p_registers, operands[2], temp1+temp2);

	} else if (operator == I_ADDI) {
		temp1 = read_from_register(p_registers, operands[0]);
		write_to_register(p_registers, operands[1], temp1+operands[2]);

	} else if (operator == I_AND) {
		temp1 = read_from_register(p_registers, operands[1]);
		temp2 = read_from_register(p_registers, operands[0]);

		write_to_register(p_registers, operands[2], temp1 & temp2);

	} else if (operator == I_BEQ) { 
		temp1 = read_from_register(p_registers, operands[0]);
		temp2 = read_from_register(p_registers, operands[1]);

		if (temp1 == temp2) {
			increase_pc(p_registers, operands[2] << 2);
			PC_increased = 1;
		}

	} else if (operator == I_BGTZ) { 
		temp1 = read_from_register(p_registers, operands[0]);

		if (temp1 > 0) {
			increase_pc(p_registers, operands[2] << 2);
			PC_increased = 1;
		}

	} else if (operator == I_BLEZ) { 
		temp1 = read_from_register(p_registers, operands[0]);

		if (temp1 <= 0) {
			increase_pc(p_registers, operands[2] << 2);
			PC_increased = 1;
		}

	} else if (operator == I_BNE) { 
		temp1 = read_from_register(p_registers, operands[0]);
		temp2 = read_from_register(p_registers, operands[1]);

		if (temp1 != temp2) {
			increase_pc(p_registers, operands[2] << 2);
			PC_increased = 1;
		}
		

	} else if (operator == I_DIV) { 
		temp1 = read_from_register(p_registers, operands[0]);
		temp2 = read_from_register(p_registers, operands[1]);

		if (temp2 == 0)
		{
			printf("Divide-by-zero detected. Cancel Action !\n");
		}else{
			write_to_register(p_registers, I_MFLO, temp1/temp2);
			write_to_register(p_registers, I_MFHI, temp1%temp2);
		}


	} else if (operator == I_J) { 
		write_to_register(p_registers, R_PC, operands[0] << 2);
		PC_increased = 1;

	} else if (operator == I_JAL) {
		temp1 = read_from_register(p_registers, R_PC);

		write_to_register(p_registers, 31, temp1+4);
		write_to_register(p_registers, R_PC, operands[0] << 2);
		PC_increased = 1;

	} else if (operator == I_JR) {
		temp1 = read_from_register(p_registers, operands[0]);

		write_to_register(p_registers, R_PC, temp1 && 0xFFFFFFFC);

		PC_increased = 1;

	} else if (operator == I_LUI) {
		write_to_register(p_registers, operands[0], operands[1] << 16);

	} else if (operator == I_LW) {
		temp2 = read_from_register(p_registers, operands[0]) + operands[2];
		temp1 = 0;
		temp1 += (read_from_memory(p_memory, temp2) << 24);
		temp1 += (read_from_memory(p_memory, temp2 + 1) << 16);
		temp1 += (read_from_memory(p_memory, temp2 + 2) << 8);
		temp1 += (read_from_memory(p_memory, temp2 + 3));

		write_to_register(p_registers, operands[1], temp1);

	} else if (operator == I_MFHI) {
		temp1 = read_from_register(p_registers, R_HI);

		write_to_register(p_registers, operands[2], temp1);

	} else if (operator == I_MFLO) {
		temp1 = read_from_register(p_registers, R_LO);

		write_to_register(p_registers, operands[2], temp1); 

	} else if (operator == I_MULT) { 

		temp1 = read_from_register(p_registers, operands[0]);
		temp2 = read_from_register(p_registers, operands[1]);

		tempLong = (long)temp1 * (long)temp2;
		temp1 = (int)((tempLong & 0xFFFFFFFF00000000) >> 32);
		temp2 = (int)(tempLong & 0x00000000FFFFFFFF);

		write_to_register(p_registers, R_HI, temp1);
		write_to_register(p_registers, R_LO, temp2);

	} else if (operator == I_OR) {
		temp1 = read_from_register(p_registers, operands[0]);
		temp2 = read_from_register(p_registers, operands[1]);

		write_to_register(p_registers, operands[2], temp1 | temp2);

	} else if (operator == I_ROTR) { 
		temp1 = read_from_register(p_registers, operands[0]);
		temp2 = read_from_register(p_registers, operands[2]);

		for (i = 0; i < temp2; ++i) {
			temp3 = (temp1 & 1) << 31;
			temp2 = temp2 >> 1;
			temp2 += temp3;
		}

		write_to_register(p_registers, operands[1], temp2);

	} else if (operator == I_SLL) {
		temp1 = read_from_register(p_registers, operands[0]);

		write_to_register(p_registers, operands[1], temp1 << operands[2]);

	} else if (operator == I_SLT) {
		temp1 = read_from_register(p_registers, operands[0]);
		temp2 = read_from_register(p_registers, operands[1]);

		write_to_register(p_registers, operands[2], temp1<temp2);

	} else if (operator == I_SRL) {
		temp1 = read_from_register(p_registers, operands[0]);

		write_to_register(p_registers, operands[1], temp1 >> operands[2]); 

	} else if (operator == I_SUB) { 
		temp1 = read_from_register(p_registers, operands[0]);
		temp2 = read_from_register(p_registers, operands[1]);

		write_to_register(p_registers, operands[2], temp1 - temp2);

	} else if (operator == I_SW) {
		temp2 = read_from_register(p_registers, operands[0]) + operands[2];
		temp1 = read_from_register(p_registers, operands[1]);
		
		write_to_memory(p_memory, temp2, (temp1 & 0xFF000000) >> 24);
		write_to_memory(p_memory, temp2 + 1, (temp1 & 0x00FF0000) >> 16);
		write_to_memory(p_memory, temp2 + 2, (temp1 & 0x0000FF00) >> 8);
		write_to_memory(p_memory, temp2 + 3, (temp1 & 0x000000FF));

	} else if (operator == I_SYSCALL) {
		printf("SignalException(SystemCall)\n");

	} else if (operator == I_XOR) {
		temp1 = read_from_register(p_registers, operands[0]);
		temp2 = read_from_register(p_registers, operands[1]);

		write_to_register(p_registers, operands[2], temp1 ^ temp2);

	}

	/* 4 - We update the PC */
	if (!PC_increased) {
		
		increase_pc(p_registers, 4);
		PC_increased = 0;
	}

	return;
}

void get_instruction_values(int instruction, unsigned int* p_operator, int* p_operands){


	/* 1 - Getting the oppcode with all specials and ROTR bit*/
	unsigned int oppcode = instruction & 0xFC000000;
	if ( oppcode == 0)
	{
		oppcode = instruction & 0x3F;
	}
	*p_operator = oppcode;

	if ( oppcode == I_SRL && ((instruction & 0x200000) == 0x200000))
	{
		oppcode = I_ROTR;
	}

	/*We are getting all operands (Some will be 0 because not used for each instruction)*/ 
	if ( oppcode == I_ADD || oppcode == I_AND || oppcode == I_DIV || oppcode == I_JR
	 || oppcode == I_MFHI || oppcode == I_MFLO || oppcode == I_MULT || oppcode == I_OR
	 || oppcode == I_SLT || oppcode == I_SUB || oppcode == I_XOR)
	{
		p_operands[0] = (instruction & 0x3E00000) >> 21;
		
		p_operands[1] = (instruction & 0x1F0000) >> 16;
		p_operands[2] = (instruction & 0xF800) >> 11;
	}
	else if ( oppcode == I_ADDI || oppcode == I_BEQ || oppcode == I_BGTZ || oppcode == I_BLEZ
	 || oppcode == I_BNE || oppcode == I_LUI || oppcode == I_LW || oppcode == I_SW){
		p_operands[0] = (instruction & 0x3E00000) >> 21;
		p_operands[1] = (instruction & 0x1F0000) >> 16;
		p_operands[2] = instruction & 0xFFFF;
		if (p_operands[2] >= 0x8000 && oppcode == I_ADDI)
		{
			p_operands[2] += 0xFFFF0000;
		}
		
	}
	else if( oppcode == I_J || oppcode == I_JAL){
		p_operands[0] = instruction & 0x3FFFFFF;
	}
	else if( oppcode == I_ROTR || oppcode == I_SLL || oppcode == I_SRL){
		p_operands[0] = (instruction & 0x1F0000) >> 16;
		p_operands[1] = (instruction & 0xF800) >> 11;
		p_operands[2] = (instruction & 0x7C0) >> 6;
	}
}

