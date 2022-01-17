#ifndef __TABLE_H__
#define __TABLE_H__


	/* The oppcode of each instruction (Shifted by 26 for non-special one). */
	static int instructionOppCode[26] = {
		I_ADD,I_ADDI,I_AND,I_BEQ,I_BGTZ,
		I_BLEZ,I_BNE,I_DIV,I_J,I_JAL,I_JR,
		I_LUI,I_LW,I_MFHI,I_MFLO,I_MULT,
		I_NOP,I_OR,I_ROTR,I_SLL,I_SLT,I_SRL,
		I_SUB,I_SW,I_SYSCALL,I_XOR
	};

	/* The number of bits the operand needs to be shifted for each instruction. */
	static int instructionShift[26][3] = {
		{11,21,16},{16,21,0},{11,21,16},{21,16,0},{21,0,0},
		{21,0,0},{21,16,0},{21,16,0},{0,0,0},{0,0,0},{21,0,0},
		{16,0,0},{16,0,21},{11,0,0},{11,0,0},{21,16,0},
		{0,0,0},{11,21,16},{11,16,6},{11,16,6},{11,21,16},{11,16,6},
		{11,21,16},{16,0,21},{0,0,0},{11,21,16}
	};

	/* Name of each instruction */
	static char instructionName[26][8] = {
		"ADD","ADDI","AND","BEQ","BGTZ",
		"BLEZ","BNE","DIV","J","JAL","JR",
		"LUI","LW","MFHI","MFLO","MULT",
		"NOP","OR","ROTR","SLL","SLT","SRL",
		"SUB","SW","SYSCALL","XOR"
	};

	/* The order of each operand for each instruction. */
	static int instructionOrder[26][3] = {
		{2,0,1},{1,0,2},{2,0,1},{0,1,2},{0,2,-1},
		{0,2,-1},{0,1,2},{0,1,-1},{0,-1,-1},{0,-1,-1},{0,-1,-1},
		{1,2,-1},{1,2,0},{2,-1,-1},{2,-1,-1},{0,1,-1},
		{-1,-1,-1},{2,0,1},{1,0,2},{1,0,2},{2,0,1},{1,0,2},
		{2,0,1},{1,2,0},{-1,-1,-1},{2,0,1}
	};

	/* The type of each operand for each instruction. */
	static int instructionIsRegister[26][3] = {
		{3,3,3},{3,3,0},{3,3,3},{3,3,0},{3,0},
		{3,0},{3,3,0},{3,3},{1},{1},{3},
		{3,0},{3,0,4},{3},{3},{0,3},
		{},{3,3,3},{3,3,2},{3,3,2},{3,3,3},{3,3,2},
		{3,3,3},{3,0,4},{},{3,3,3}
	};

	/* Masks corresponding to each operand size. */
	static int maskTab[3] = {
		0xFFFF,0x3FFFFFF,0x1F
	};

	/* Name of each register */
	static char registersTable[32][6] = {
	"$ZERO",
	"$AT",
	"$V0","$V1",
	"$A0","$A1","$A2","$A3",
	"$T0","$T1","$T2","$T3","$T4","$T5","$T6","$T7",
	"$S0","$S1","$S2","$S3","$S4","$S5","$S6","$S7",
	"$T8","$T9",
	"$K0","$K1",
	"$GP","$SP","$FP","$RA"
	};



#endif