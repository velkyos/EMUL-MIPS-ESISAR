#Le but est de remplir progressivement la mémoire de l'adresse 20 jusqu'à 0 de FF.
addi $t0, $0, 20
addi $t1, $0, 0xFF

BLEZ $t0, 4 
  sw $t1, 0($t0)
  addi $t0, $t0, -1
  J 2 #On revient au début

lw $s0, 0($t0)