/* virtual machine*/
#include "VM.h"

int main(int argc,char* argv[])
{
	FILE* inFile;
	VirtualMachine vm;

	vm.stackPtr = 0;
	vm.instructionPtr = 0;

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
	unsigned char reg1,reg2;
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
				reg1 = ((*vm).instructions[(*vm).instructionPtr+1]);
				Load(vm,reg1);
				(*vm).instructionPtr += 2;
				break;
			case 0x04:
				reg1 = ((*vm).instructions[(*vm).instructionPtr+1]);
				Store(vm,reg1);
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
				reg1 = ((*vm).instructions[(*vm).instructionPtr+1]);
				reg2 = ((*vm).instructions[(*vm).instructionPtr+2]);
				Add(vm,reg1,reg2);
				(*vm).instructionPtr+=3;
				break;
			case 0xA:
				reg1 = ((*vm).instructions[(*vm).instructionPtr+1]);
				arg  = (int)((*vm).instructions[(*vm).instructionPtr+2]);
				Add(vm,reg1,arg);
				(*vm).instructionPtr+=6;
				break;
			case 0xB:
				Sub(vm);
				(*vm).instructionPtr++;
				break;
			case 0xC:
				reg1 = ((*vm).instructions[(*vm).instructionPtr+1]);
				reg2 = ((*vm).instructions[(*vm).instructionPtr+2]);
				Sub(vm,reg1,reg2);
				(*vm).instructionPtr+=3;
				break;
			case 0xD:
				reg1 = ((*vm).instructions[(*vm).instructionPtr+1]);
				arg  = (int)((*vm).instructions[(*vm).instructionPtr+2]);
				Sub(vm,reg1,arg);
				(*vm).instructionPtr+=6;
				break;
			case 0xE:
				Mul(vm);
				(*vm).instructionPtr++;
				break;
			case 0xF:
				reg1 = ((*vm).instructions[(*vm).instructionPtr+1]);
				reg2 = ((*vm).instructions[(*vm).instructionPtr+2]);
				Mul(vm,reg1,reg2);
				(*vm).instructionPtr+=3;
				break;
			case 0x10:
				reg1 = ((*vm).instructions[(*vm).instructionPtr+1]);
				arg  = (int)((*vm).instructions[(*vm).instructionPtr+2]);
				Mul(vm,reg1,arg);
				(*vm).instructionPtr+=6;
				break;
			case 0x11:
				Div(vm);
				(*vm).instructionPtr++;
				break;
			case 0x12:
				reg1 = ((*vm).instructions[(*vm).instructionPtr+1]);
				reg2 = ((*vm).instructions[(*vm).instructionPtr+2]);
				Div(vm,reg1,reg2);
				(*vm).instructionPtr+=3;
				break;
			case 0x13:
				reg1 = ((*vm).instructions[(*vm).instructionPtr+1]);
				arg  = (int)((*vm).instructions[(*vm).instructionPtr+2]);
				Div(vm,reg1,arg);
				(*vm).instructionPtr+=6;
				break;
			case 0x14:
				Print(vm);
				(*vm).instructionPtr++;
				break;
			case 0x15:
				/* stop*/
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

void Add(VirtualMachine* vm,unsigned char reg1,unsigned char reg2)
{
	(*vm).registers[reg1] += (*vm).registers[reg2];
}

void Add(VirtualMachine* vm,unsigned char reg1,int arg)
{
	(*vm).registers[reg1] += arg;
}

void Sub(VirtualMachine* vm)
{
	int arg1 = Pop(vm);
	int arg2 = Pop(vm);

	Push(vm,arg2-arg1);
}

void Sub(VirtualMachine* vm,unsigned char reg1,unsigned char reg2)
{
	(*vm).registers[reg1] -= (*vm).registers[reg2];
}

void Sub(VirtualMachine* vm,unsigned char reg1,int arg)
{
	(*vm).registers[reg1] -= arg;
}

void Mul(VirtualMachine* vm)
{
	int arg1 = Pop(vm);
	int arg2 = Pop(vm);

	Push(vm,arg1*arg2);
}

void Mul(VirtualMachine* vm,unsigned char reg1,unsigned char reg2)
{
	(*vm).registers[reg1] *= (*vm).registers[reg2];
}

void Mul(VirtualMachine* vm,unsigned char reg1,int arg)
{
	(*vm).registers[reg1] *= arg;
}

void Div(VirtualMachine* vm)
{
	int arg1 = Pop(vm);
	int arg2 = Pop(vm);

	Push(vm,arg2/arg1);
}

void Div(VirtualMachine* vm,unsigned char reg1,unsigned char reg2)
{
	(*vm).registers[reg1] /= (*vm).registers[reg2];
}

void Div(VirtualMachine* vm,unsigned char reg1,int arg)
{
	(*vm).registers[reg1] /= arg;
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