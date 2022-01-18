addi $t0, $zero, 26
addi $t1, $zero, 0xAE0
addi $t2, $zero, 0xCB9
# ...
sw $t0, 0x1($t1)
addi $t0, $t0, 83
# ...
sw $t0, 0x2($t1)
# ...
lw $t3, 0x1($t1)
sw $t3, 0x10($t2)
# ...
lw $t3, 0x2($t1)
add $t4, $zero, $t3
lw $t3, 0x10($t2)
add $t4, $t4, $t3
sw $t4, 0x1($t1)