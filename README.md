FirstVirtualMachine
===================

My first attempt at a virtual machine that runs byte code, will extend over time

The initial vm was based on material from a C course at California Tech university and their last assignment.
The track can be found here: 

http://courses.cms.caltech.edu/cs11/material/c/mike/

and the assignment itself here:

http://courses.cms.caltech.edu/cs11/material/c/mike/lab8/lab8.html

A quick rundown of the current (as of 2014/03/28) runnable commands

NOP (0x00)

"no operation"; do nothing.

PUSH (0x01)

PUSH <n> means to push <n> onto the top of the stack (TOS). <n> is a standard 4-byte integer (int).

POP (0x02)

Pop the top of the stack.

LOAD (0x03)

LOAD <r> means to load the value in register <r> to the TOS. Here <r> is a one-byte unsigned integer (see below).

STORE (0x04)

STORE <r> means to store the TOS to register <r> and pop the TOS. Again, <r> is a one-byte unsigned integer.

JMP (0x05)

"Jump"; JMP <i> means to go to location <i> in the instruction array (i.e. change the instruction pointer to <i>). <i> is a two-byte unsigned integer (see below). This is also the case for the next two instructions.

JZ (0x06)

"Jump if top of stack is zero"; JZ <i> means that if the TOS is zero, pop the TOS and go to location <i> in the instruction array. If the TOS is not zero, just pop the TOS and continue with the next instruction.

JNZ (0x07)

"Jump if top of stack is nonzero"; JNZ <i> means that if the TOS is nonzero, pop the TOS and go to location <i> in the instruction array. If the TOS is zero, just pop the TOS and continue with the next instruction.

ADD (0x08)

Pop the top two elements on the stack and replace them with their sum (S2 + S1, where S1 is the TOS and S2 is the element on the stack below the TOS).

SUB (0x09)

Pop the top two elements on the stack and replace them with their difference (S2 - S1).

MUL (0x0a)

Pop the top two elements on the stack and replace them with their product (S2 * S1).

DIV (0x0b)

Pop the top two elements on the stack and replace them with S2 / S1 (integer division).

PRINT (0x0c)

Print the TOS to stdout (followed by a newline) and pop the TOS.

STOP (0x0d)

Halt the program.

Will extend the vm in the future, especially the arithmetic commands so that instead of using values on the stack they can use 2 arguments like

ADD val1,val2

Example bytecode, assembly files and a compiler can be found at the track homepage linked above.

