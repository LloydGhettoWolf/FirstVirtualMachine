/*VM.h*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#define MEMORY_SIZE    65536
#define NUM_REGISTERS  16
#define STACK_ENTRIES  256

typedef struct _vm
{
	unsigned char instructions[MEMORY_SIZE];
	int registers[NUM_REGISTERS];
	int stack[STACK_ENTRIES];

	unsigned short instructionPtr;
	unsigned char  stackPtr;
}VirtualMachine;

void LoadProgram(VirtualMachine* vm,FILE* file);
void Execute(VirtualMachine* vm);
void Push(VirtualMachine* vm,int value);
int  Pop(VirtualMachine* vm);
void Load(VirtualMachine* vm,unsigned char reg);
void Store(VirtualMachine* vm,unsigned char reg);
void Jump(VirtualMachine* vm,unsigned short instructionIndex);
void JumpIfZero(VirtualMachine* vm,unsigned short instructionIndex);
void JumpIfNotZero(VirtualMachine* vm,unsigned short instructionIndex);
void Add(VirtualMachine* vm);
void Add(VirtualMachine* vm,unsigned char reg1,unsigned char reg2);
void Add(VirtualMachine* vm,unsigned char reg1,int arg);
void Sub(VirtualMachine* vm);
void Sub(VirtualMachine* vm,unsigned char reg1,unsigned char reg2);
void Sub(VirtualMachine* vm,unsigned char reg1,int arg);
void Mul(VirtualMachine* vm);
void Mul(VirtualMachine* vm,unsigned char reg1,unsigned char reg2);
void Mul(VirtualMachine* vm,unsigned char reg1,int arg);
void Div(VirtualMachine* vm);
void Div(VirtualMachine* vm,unsigned char reg1,unsigned char reg2);
void Div(VirtualMachine* vm,unsigned char reg1,int arg);
void Print(VirtualMachine* vm);
void InitVM(VirtualMachine* vm);