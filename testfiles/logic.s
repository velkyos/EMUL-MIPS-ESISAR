# Tests for logical operations

addi $t0, $zero, 0x18
addi $t1, $zero, 0xA3
addi $t2, $zero, 0x3E

xor $t3, $t0, $t1
and $t4, $t2, $t3
LUI $0, 0xABCD
or $t4, $t4, t0
addi $t0, $zero, 0xFAFC
and $t2, $t4, $t0
addi $t0, $zero, 4
rotr $t2, $t0, $t0
# ...
xor $t3, $t3, $t2
bgtz $t3, 2
and $t0, $t0, $t4
xor $t1, $t1, $t4
or $t3, $t3, $t1
slt $t1, $t3, $t4
xor $t3, $t3, $t1