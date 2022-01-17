Made by Adrien Louvrier, Sophie Dreano and Benjamin Robert

---
title: CS351 Esisar -- MIPS Emulator User Documentation
subtitle: Step 2, 2021-2022 
author: Louvrier Adrien, Dreano Sophie, Robert Benjamin
papersize: A4
geometry: margin=1.5cm
fontsize: 12pt
lang: en
---

<div style="text-align: center;">

## MIPS Emulator User Documentation

</div>

# **Instruction**
<div style="text-align: center;">
There are 2 modes to execute this emulator.

## **Interactive** :
***`/emul-mips`***

Need to enter instructions by instructions.

`EXIT` to quit this mode.

## **Non-Interactive** : 

***`/emul-mips prog_filename <-pas>`***

Compile the file **prog_filename** and generate a file named **output.hex** and delete after reading it and then execute all the instructions.

***`/emul-mips prog_filename outputname1 outputname2`***

Compile the file **prog_filename** and save it into **outputname1**.

Execute the programme and save the final state in **outputname2**.

***`/emul-mips prog_filename <-compile>`***

Compile the file **prog_filename** and save the binary file into **output_debug.hex**.
</div>


# **Known bugs/undef behavior**
<div style="text-align: center;">

**ADD / ADDI / SUB** : Overflow are not detected and not stop, so one bit can be lost.

**DIV** : Division with 0 is detected and canceled.

**JR** : We don't look at two leasts significants bits. We only take the multiple of 4.

Before each execution we show the instruction in 'Assembly format', the NOP instruction will not be shown instead the SLL $0, $0, 0 will.
</div>
