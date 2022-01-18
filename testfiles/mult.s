#Test multiplication 32 x 18 avec mult et l'autre avec boucle addition
#On veut que s0(17) et s1(18) on la même valeur

addi $t0, $0, 0x20
addi $t1, $0, 18

mult $t0, $t1

MFLO $s0

addi $s1, $0, 0
#début boucle
BLEZ $t0, 4 
  add $s1, $s1, $t1
  addi $t0, $t0, -1
  J 6 #On revient au début
NOP
