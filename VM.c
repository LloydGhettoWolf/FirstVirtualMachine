/* virtual machine*/
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
void Sub(VirtualMachine* vm);
void Mul(VirtualMachine* vm);
void Div(VirtualMachine* vm);
void Print(VirtualMachine* vm);
void DataCheck();
void InitVM(VirtualMachine* vm);

int main(int argc,char* argv[])
{
	FILE* inFile;
	VirtualMachine vm;

	vm.stackPtr = 0;
	vm.instructionPtr = 0;

	DataCheck();
	InitVM(&vm);

	/*check that a file name has been given*/
	if(argc < 2)
	{
		fprintf(stderr,"Usage: %s program file name",argv[0]);
		return 1;
	}

	inFile = fopen(argv[1],"r");

	LoadProgram(&vm,inFile);

	Execute(&vm);

	fclose(inFile);
	return 0;
}

void LoadProgram(VirtualMachine* vm,FILE* file)
{
	int nRead;
	unsigned char* instrSection = (*vm).instructions;

	do
	{
		nRead = fread(instrSection,1,1,file);
		instrSection++;
	}while(nRead > 0);

}

void Execute(VirtualMachine* vm)
{
	int arg;
	unsigned char reg;
	unsigned short instruction;

	while(1)
	{
		switch((*vm).instructions[(*vm).instructionPtr])
		{
			case 0x00:
				/* NOP or No operation*/
				(*vm).instructionPtr++;
				break;
			case 0x01:
				arg = (int)((*vm).instructions[(*vm).instructionPtr+1]);
				Push(vm,arg);
				(*vm).instructionPtr += 5;
				break;
			case 0x02:
				Pop(vm);
				(*vm).instructionPtr++;
				break;
			case 0x03:
				reg = ((*vm).instructions[(*vm).instructionPtr+1]);
				Load(vm,reg);
				(*vm).instructionPtr += 2;
				break;
			case 0x04:
				reg = ((*vm).instructions[(*vm).instructionPtr+1]);
				Store(vm,reg);
				(*vm).instructionPtr += 2;
				break;
			case 0x05:
				instruction = (unsigned short)((*vm).instructions[(*vm).instructionPtr+1]);
				Jump(vm,instruction);
				break;
			case 0x06:
				instruction = (unsigned short)((*vm).instructions[(*vm).instructionPtr+1]);
				JumpIfZero(vm,instruction);
				break;
			case 0x07:
				instruction = (unsigned short)((*vm).instructions[(*vm).instructionPtr+1]);
				JumpIfNotZero(vm,instruction);
				break;
			case 0x08:
				Add(vm);
				(*vm).instructionPtr++;
				break;
			case 0x09:
				Sub(vm);
				(*vm).instructionPtr++;
				break;
			case 0xA:
				Mul(vm);
				(*vm).instructionPtr++;
				break;
			case 0xB:
				Div(vm);
				(*vm).instructionPtr++;
				break;
			case 0xC:
				Print(vm);
				(*vm).instructionPtr++;
				break;
			case 0xD:
				return;
			default:
				return;
		}
	}
}

void Push(VirtualMachine* vm,int value)
{
	assert((*vm).stackPtr < STACK_ENTRIES && "stack overflow!") ;
	(*vm).stack[(*vm).stackPtr++] = value;
}

int  Pop(VirtualMachine* vm)
{
	int arg;
	assert((*vm).stackPtr >= 0 && "stack underflow!") ;
	arg = (*vm).stack[(*vm).stackPtr-1];
	(*vm).stackPtr--;
	return arg;
}

void Load(VirtualMachine* vm,unsigned char reg)
{
	assert(reg >= 0 && reg < 16 && "trying to access a non existant register!");
	Push(vm,(*vm).registers[reg]);
}

void Store(VirtualMachine* vm,unsigned char reg)
{
	assert(reg >= 0 && reg < 16 && "trying to access a non existant register!");
	(*vm).registers[reg] = Pop(vm);
}

void Jump(VirtualMachine* vm,unsigned short instructionIndex)
{
	assert(instructionIndex >= 0 && instructionIndex < MEMORY_SIZE && "out of bounds mem access");
	(*vm).instructionPtr = instructionIndex;
}

void JumpIfZero(VirtualMachine* vm,unsigned short instructionIndex)
{
	assert(instructionIndex >= 0 && instructionIndex < MEMORY_SIZE && "out of bounds mem access");

	if((*vm).stack[(*vm).stackPtr-1] == 0)
	{
		(*vm).instructionPtr = instructionIndex;
	}
	else
	{
		(*vm).instructionPtr+=3;
	}
}

void JumpIfNotZero(VirtualMachine* vm,unsigned short instructionIndex)
{
	assert(instructionIndex >= 0 && instructionIndex < MEMORY_SIZE && "out of bounds mem access");

	if((*vm).stack[(*vm).stackPtr-1] != 0)
	{
		(*vm).instructionPtr = instructionIndex;
	}
	else
	{
		(*vm).instructionPtr+=3;
	}
}

void Add(VirtualMachine* vm)
{
	int arg1 = Pop(vm);
	int arg2 = Pop(vm);

	Push(vm,arg1+arg2);
}

void Sub(VirtualMachine* vm)
{
	int arg1 = Pop(vm);
	int arg2 = Pop(vm);

	Push(vm,arg2-arg1);
}

void Mul(VirtualMachine* vm)
{
	int arg1 = Pop(vm);
	int arg2 = Pop(vm);

	Push(vm,arg1*arg2);
}

void Div(VirtualMachine* vm)
{
	int arg1 = Pop(vm);
	int arg2 = Pop(vm);

	Push(vm,arg2/arg1);
}

void Print(VirtualMachine* vm)
{
	int arg = Pop(vm);
	printf("%d \n",arg);
}


void InitVM(VirtualMachine* vm)
{
	int loop;

	for(loop = 0; loop < STACK_ENTRIES; loop++)
	{
		(*vm).stack[loop] = 0;
	}

	for(loop = 0; loop < MEMORY_SIZE; loop++)
	{
		(*vm).instructions[loop] = 0;
	}
}