# Tests for syscall and nop instructions
ADDI $t0, $zero, 0xAC # a = 0xAC
ADDI $t1, $zero, 0xDC # b = 0xDC
NOP
SUB $t2, $t1, $t0 # c = b - a
SYSCALL