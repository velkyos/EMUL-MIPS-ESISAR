#include "registers.h"

void write_to_register(Registers *p_registers, int registerId, int value){
	switch (registerId){
		case R_PC:
			p_registers->PC = value;
			break;
		case R_HI:
			p_registers->HI = value;
			break;
		case R_LO:
			p_registers->LO = value;
			break;
		case 0:
			break;
		default:
			p_registers->GPR[registerId] = value;
			p_registers->haveModif[registerId] = 1;
	}
	
}

int read_from_register(Registers *p_registers, int registerId){
	int p_value = 0;
	switch (registerId){
		case R_PC:
			p_value = p_registers->PC;
			break;
		case R_HI:
			p_value = p_registers->HI;
			break;
		case R_LO:
			p_value = p_registers->LO;
			break;
		default:
			p_value = p_registers->GPR[registerId];
	}
	return p_value;
}

void init_registers(Registers *p_registers){
	for (int i = 0; i < REGISTER_SIZE; i++)
	{
		p_registers->haveModif[i] = 0;
		p_registers->GPR[i] = 0;
	}
	p_registers->haveModif[0] = 1;
	p_registers->PC = 0;
}

void increase_pc(Registers *p_registers, int value){
	write_to_register( p_registers , R_PC , read_from_register(p_registers, R_PC) + value );
}

int count_modified_registers(Registers *p_registers){
	int value = 0;
	for (int i = 0; i < REGISTER_SIZE; i++)
	{
		value += p_registers->haveModif[i] == 1;
	}
	return value;
}

int get_next_modified_register(Registers *p_registers, int last){
	int i = ++last;
	while ( !p_registers->haveModif[i] ){
		i++;
	}
	return i;
}

void print_registers_to_file(FILE *p_file, Registers *p_registers){
	int i = 0;
	
	for ( i = 0; i < REGISTER_SIZE; i++)
	{
		fprintf( p_file, "$%02d:%d\n", i, read_from_register(p_registers, i));
	}
	fprintf( p_file, "HI:%d\n", read_from_register(p_registers, R_HI));
	fprintf( p_file, "LO:%d\n", read_from_register(p_registers, R_LO));
}
