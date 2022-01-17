#include "assembly_compilation.h"
#include "tables.h"

#define DEBUG 0

/* Open a file named {p_filename} with the mode {p_mode} */
FILE* open_File(char *p_filename, char*p_mode);
/* Compile the instruction {p_instruction} and store the result in {p_rest} return an error code*/
int compile_instruction(char *p_instruction, int *p_res);

/* Section - Operands & Operator*/

/* Get the operator {p_operator} and the operands {p_operands} from the line {p_line} return an error code*/
int get_operands_operator(char *p_ligne, char *p_operator, int *p_operands);
/* Get the {p_operator} (integer) from the line {p_line} and modify the index {p_index} return an error code*/
void get_operator(char *p_ligne, char *p_operator, int *p_index);
/* Get the {p_operand} (integer) from the line {p_line} and modify the index {p_index} return an error code*/
int get_operands(char *p_ligne, int *p_operand, int *p_index);
/* Get the {p_operand} (string) from the line {p_line} and modify the index {p_index} return an error code*/
int get_string_operand(char *p_ligne, char *p_operand, int *p_index);
/* Convert a string {p_operandString} (Hexa or decimal) into an integer {p_operand} return an error code*/
int string_to_int(char *p_operandString, int *p_operand);
/* Convert a string {p_operandString} (Register) into an integer {p_operand} return an error code*/
int register_to_int(char *p_operandString, int *p_operand);
/* Convert a string {p_operandString} from a start char {start} into an integer {p_operand} with a base {start} (10 or 16) return an error code*/
int base_to_int(char *p_operandString, int *p_operand,int start, int base);

/* Section - Line Edition */

/* Modified the index to skip empty space between operands*/
void remove_empty_space(char *p_ligne, int *p_index);

/* Put all lower case in UpperCase */
char *to_Upper_case(char *p_line);
/* Remove space before and after the line */
void clean_Line(char* p_word);
/* Remove comment from the line */
void remove_Comments(char* p_word);


/*
*
*
*/


FILE* open_File(char *p_filename, char*p_mode){
	FILE* file = fopen(p_filename, p_mode);
	if (file == NULL){
		printf("Error while opening %s\n", p_filename);
		exit(-1);
	}
	return file;
}

void write_final_file(char *p_path, Registers *p_registers){
	FILE* fileOut = open_File(p_path, "w");
	if ( fileOut != NULL)
	{
		print_registers_to_file(fileOut, p_registers);
		fclose(fileOut);
	}
	 else {
		perror("write_final_file -fileOut null");
	}
	
}

void load_compiled_file(char *p_pathIn, int *p_program, int needDelete){
	int index = 0;
	int instruction = 0;
	char buffer[9];
	/* 1 - Open the file */
	FILE* fileIn = open_File(p_pathIn, "r");

	if (fileIn != NULL) {
		/* 2 - Read all line and store them into a list of int */
		while (!feof(fileIn)) {
			fgets(buffer, 9, fileIn);
			if (!( buffer[0] == '\n' || buffer[0] == '\0' ))
			{
				buffer[8] = '\0';
				/* 2.5 - Convert the Hexa string into integer */
				base_to_int(buffer, &instruction, 0, 16);
				p_program[index++] = instruction;

			}
		}
		/* 3 - Close the file */
		fclose(fileIn);
		
		/* 3.5 - Delete the tempory file if needed */
		if (needDelete) remove(p_pathIn);	
		
	} else {
		perror("load_compiled_file - fileIn null");
	}
}

int compile_line(char *p_line, int *p_instruction){
	int error = EMPTY_LINE;

	if ( DEBUG ) printf("-- \nLigne avant modif : %s", p_line);

	remove_Comments(p_line);

	if ( DEBUG ) printf("Ligne sans commentaires : %s\n", p_line);

	clean_Line(p_line);

	if ( DEBUG ) printf("Ligne sans espaces vides : %s\n", p_line);

	/* if the line as assembly code */
	if (p_line[0] != '\0' && p_line[0] != '\n' && p_line[0] != '\r') {

		to_Upper_case(p_line);

		if ( DEBUG ) printf("Ligne avant compilation : %s\n", p_line);

		/* We execute the line and return the error code */
		error = compile_instruction(p_line, p_instruction);
	}

	return error;
}

int compile_file(char *p_pathIn, char *p_pathOut){

	int instruction;
	int ligne = 0;
	int nbrInstruction = 0;
	int error = NO_ERROR;
	char buffer[BUFFER_SIZE];

	/* 1 - Open the files */
	FILE* fileIn = open_File(p_pathIn, "r");
	FILE* fileOut = open_File(p_pathOut, "w");
	
	if (fileIn != NULL && fileOut != NULL) {

		printf("The compilation has begun %s ...\n", p_pathIn);

		/* 2 - Read the source file */
		while (!feof(fileIn)) {
			ligne++;

			fgets(buffer, BUFFER_SIZE, fileIn);

			/* 3 - Compile each line */
			error = compile_line(buffer, &instruction);

			/* 4 - Write into the .hex file if no error */
			if ( error == NO_ERROR) {
				fprintf(fileOut, "%.8X\n", instruction);
				nbrInstruction ++;
			}
			
			/* 5 - Show errors */
			handle_errors(error, ligne);

			buffer[0] = '\0'; //Security to prevent duplication of instruction
		}
		
		/* 6 - End */
		printf("Success ! File created : %s\n", p_pathOut);
		fclose(fileOut);
		fclose(fileIn);
		
	} else {

		perror("compile_file - fileIn or fileOut null");

	}
	return nbrInstruction;
}

void decompile_instruction(char ** s_output, int operator,int* p_operands){
	int i = 0;
	int order;
	char s_operator[15];
	char s_operands[3][15];

	char *temp1 = NULL;
	char *temp2 = NULL;

	/* Checking for oppcode */
	while (instructionOppCode[i] != operator && i < NBR_INSTRUCTION)
	{
		i++;
	}
	i = (operator == I_NOP) ? i + 3 : i; /* Getting SLL instead of NOP */

	sprintf(s_operator, "%s", instructionName[i]);
	
	/* Getting Operands Values */
	for (int y = 0; y < 3; y++)
	{
		order = instructionOrder[i][y];
		if ( order != -1)
		{
			switch ( instructionIsRegister[i][y])
			{
			case 3:
				sprintf(s_operands[y], ", %s", registersTable[p_operands[order]]);
				break;
			case 4:
				sprintf(s_operands[y], "(%s)", registersTable[p_operands[order]]);
				break;
			default:
				sprintf(s_operands[y], ", 0x%.x", p_operands[order]);
				break;
			}
		}
	}
	
	string_concat(&temp1, s_operator, s_operands[0]);
	string_concat(&temp2, s_operands[1], s_operands[2]);
	string_concat(s_output, temp1, temp2);
	
	free(temp1);
	free(temp2);
}

int compile_instruction(char *p_instruction, int *p_res){
	char operator[MAX_OPERATOR_SIZE];
	int operands[3] = {0,0,0};
	int error = NO_ERROR;
	

	*p_res = 0 ;

	// pick up the operator and the operands from the instruction string
	error = get_operands_operator(p_instruction, operator, operands);

	// if there is not error, compare the operator string with each possible assembly operator
	// if the operator corresponds to an assembly operator, convert the value of the instruction in hexa
	if (error == NO_ERROR) {
		int i = 0;
		int choose = 0;
		int mask[3];
		int maskedOperands[3];
		while( i < NBR_INSTRUCTION && !choose){
			
			if( string_compare(operator, instructionName[i], 0)){

				*p_res += instructionOppCode[i];
				for (int y = 0; y < 3; y++)
				{
					mask[y] = instructionIsRegister[i][y] < 3 ? maskTab[instructionIsRegister[i][y]] : maskTab[2];
					maskedOperands[y] = operands[y] & mask[y];

					if (instructionOrder[i][y] != -1) /* Detect if this operand is used */
					{
						if ( maskedOperands[y] != operands[y] ) error = ERROR_OPERAND;  /* Detect the operand is overflowing */
						
						*p_res += (maskedOperands[y]) << instructionShift[i][y] ;
					}
				}
				choose = 1;
				
			}
			i++;
		}
		if (choose == 0)
		{
			error += ERROR_OPERATOR;
		}
	
	}
	return error;
}

int get_operands_operator(char *p_ligne, char *p_operator, int *p_operands){

	int index = 0;
	int error = NO_ERROR;
	int temp_error = 0; /* Temp is used to prevent lost of errors */
	
	get_operator(p_ligne, p_operator, &index);

	for (int i = 0; i < 3; ++i)
	{
		temp_error = get_operands(p_ligne, &p_operands[i], &index);
		if (error == NO_ERROR && error != temp_error)
		{
			error = temp_error;
		}
	}

	return error;
}

void get_operator(char *p_ligne, char *p_operator, int *p_index){
	int indexOperator = 0;

	while( p_ligne[*p_index] != ' ' && p_ligne[*p_index] != '\0' ){
		if (indexOperator < (MAX_OPERATOR_SIZE) - 1)
		{
			p_operator[indexOperator] = p_ligne[*p_index];
			indexOperator++;
		}
		else{ /* The Operator is too big to be a correct one*/
			p_operator[0] = 'E';
			p_operator[1] = 'R';
			p_operator[2] = 'O';
			p_operator[3] = 'R';
			p_operator[4] = '\0';
		}
		(*p_index)++;
	}

	remove_empty_space(p_ligne, p_index);

	p_operator[indexOperator] = '\0';
}

int get_operands(char *p_ligne, int *p_operand, int *p_index){
	char operand_String[MAX_OPERAND_SIZE];
	int error = NO_ERROR;

	error = get_string_operand(p_ligne, operand_String, p_index);

	error = string_to_int(operand_String, p_operand);

	return error;
}

int get_string_operand(char *p_ligne, char *p_operand, int *p_index){
	int indexOperand = 0;
	int error = NO_ERROR;

	remove_empty_space(p_ligne, p_index);

	while(p_ligne[*p_index] != ',' 
		&& p_ligne[*p_index] != '(' 
		&& p_ligne[*p_index] != '\0'
		&& p_ligne[*p_index] != ')'
		&& p_ligne[*p_index] != ' '
		&& indexOperand < MAX_OPERAND_SIZE){

		p_operand[indexOperand] = p_ligne[*p_index];
		indexOperand++;
		(*p_index)++;
	}

	if (indexOperand >= MAX_OPERAND_SIZE) error = ERROR_OPERAND;
	
	remove_empty_space(p_ligne, p_index);

	//Remove the ','
	if (p_ligne[*p_index] == ',' || p_ligne[*p_index] == '(')
		(*p_index)++;

	p_operand[indexOperand] = '\0';

	return error;
}

int string_to_int(char *p_operandString, int *p_operand){
	int error = NO_ERROR;
	if ( p_operandString[0] == '$')
	{
		error = register_to_int( p_operandString, p_operand);
	}
	else if ( string_compare( p_operandString, "0X" , 1) ) {
		error = base_to_int( p_operandString, p_operand, 2, 16);
	}
	else{
		error = base_to_int( p_operandString, p_operand, 0, 10);
		
	}
	return error;
}

int register_to_int(char *p_operandString, int *p_operand){
	int error = NO_ERROR;
	if ( p_operandString[1] <= '9' )
	{
		error = base_to_int( p_operandString, p_operand, 1, 10);
	}
	else{

		int i = 0;
		int choose = 0;
		while( i < REGISTER_SIZE && !choose){
			if( string_compare( p_operandString, registersTable[i], 1)){
				*p_operand = i;
				choose = 1;
			}
			i++;
		}
		if (choose == 0) error = ERROR_OPERAND;
	}
	return error;
}
	
int base_to_int(char *p_operandString, int *p_operand,int start, int base){
	int i = start;
	int value = -1;
	int isNegatif = (p_operandString[i] == '-');
	i += isNegatif; /* We start one char after */
	//First Caracter, 0 for decimal and A - 10 for Hexa nbr > 9
	int first = '0';
	int error = NO_ERROR;

	if (p_operandString[i] != '\0')
	{
		value = 0;
	}
	
	while( p_operandString[i] != '\0'){

		
		if ( p_operandString[i] <= '9' && p_operandString[i] >= '0')
		{
			first = '0';
		}
		else if ( p_operandString[i] <= 'F' && p_operandString[i] >= 'A' && base == 16)
		{
			first = 'A' - 10;
		}
		else{
			error = ERROR_OPERAND;
		}
		value = base*value + (p_operandString[i] - first);
		i++;
	}
	if (isNegatif)
	{
		value = 65536 - value;
	}

	*p_operand = value;
	return error;
}

void remove_empty_space(char *p_ligne, int *p_index){
	int i = *p_index;
	while( p_ligne[i] == ' '){
		i ++;
	}
	*p_index = i;
}

char *to_Upper_case(char *p_line){

	int counter = 0;

	while (p_line[counter] != '\0') {
		if (p_line[counter] >= 'a' && p_line[counter] <= 'z') {
			p_line[counter] = p_line[counter] - 0x20;
		}
		counter++;
	}
	return p_line; //mé
}

void clean_Line(char* p_word){

 	int i = 0;
 	int j = 0;

 	while (p_word[i] == ' ' || p_word[i] == '\t'){
 		i++;
 	}
 	//Vérifier la fin
 	while (p_word[i] != '\n' && p_word[i] != '\0' &&  p_word[i] != '\r') {

 		if ( (p_word[i] != ' ' && p_word[i] != '\t') || ( p_word[i-1] != ' ' && p_word[i-1] != '\t') ){
 			p_word[j] = p_word[i];
 			j++;
 		}
 		i++;

 	}

 	if (p_word[j-1] == ' ' || p_word[j-1] == '\t' ) {

		j--;

	}

	p_word[j] = '\0';

 }

void remove_Comments(char* p_word){

 	int i = 0;
 	while(p_word[i] != '\0' && p_word[i] != '#' &&  p_word[i] != '\r' && p_word[i] != '\n'){
 		i++;
 	}

 	p_word[i] = '\0';

 }

