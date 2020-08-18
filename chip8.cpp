#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


//function to process op codes
void DisassembleChip8Op(uint8_t* codebuffer, int pc)
{
	uint8_t* code = &codebuffer[pc];	//??
	uint8_t firstnib = (code[0] >> 4); //upper 4 bits of the first byte (opcode)

	printf("%04x %02x %02x ", pc, code[0], code[1]);

	switch (firstnib)
	{
	case 0x00: 
		switch (code[1])
		{
		case 0xe0: printf("%-10s", "CLS"); break;	//Clear screen
		case 0xee: printf("%-10s", "RTS"); break;	//Return from subroutine
		}
		break;
	case 0x01: printf("%-10s $%01x%02x", "JUMP", code[0]&0xf, code[1]); break;		   //Jump to address
	case 0x02: printf("%-10s $%01x%02x", "CALL", code[0]&0xf, code[1]); break;		   //Calls to subroutine
	case 0x03: printf("%-10s V%01X,#$%02x", "SKIP.EQ", code[0] & 0xf, code[1]); break; //Skip next instruction if VX=NN  
	case 0x04: printf("%-10s V%01X,#$%02x", "SKIP.NE", code[0]&0xf, code[1]); break;   //Skip next instruction if VX != NN
	case 0x05: printf("%-10s V%01X,V%01X", "SKIP.EQ", code[0]&0xf, code[1]); break;    //Skip next instruction if VX = VY

		// op code 6XNN movesan immediate value to a register
	case 0x06:
	{
		uint8_t reg = code[0] & 0x0f;
		printf("%-10s V%01X,#$%02x", "MVI", reg, code[1]);
	}
	break;
	case 0x07: printf("%-10s V%01X,#$%02x", "ADI", code[0]&0xf, code[1]); break;		//Adds NN to VX
	
	//Main ALU(Math) operations
	case 0x08: 
	{
		uint8_t lastnib = code[1] & 0xf;
		switch (lastnib)
		{
		case 0: printf("%-10s V%01X,V%01X", "MOV.", code[0] & 0xf, code[1] >> 4); break; //MOV operation VX=VY
		case 1: printf("%-10s V%01X,V%01X", "OR.", code[0] & 0xf, code[1] >> 4); break;  //Bitwise OR operation
		case 2: printf("%-10s V%01X,V%01X", "AND.", code[0] & 0xf, code[1] >> 4); break; //Bitwise AND operation
		case 3: printf("%-10s V%01X,V%01X", "XOR.", code[0] & 0xf, code[1] >> 4); break; //Bitwise XOR operation (Exlusive OR)
		case 4: printf("%-10s V%01X,V%01X", "ADD.", code[0] & 0xf, code[1] >> 4); break; //ADDs VY and VX, VF set to 1 if carry
		case 5: printf("%-10s V%01X,V%01X,V%01X", "SUB.", code[0] & 0xf, code[0] & 0xf, code[1] >> 4); break; //SUBs VY from VX, VF set to 0 if borrow
		case 6: printf("%-10s V%01X,V%01X", "SHR.", code[0] & 0xf, code[1] >> 4, code[1] >> 4); break; //Stores LSB of VX in VF, then shifts VX to the right by 1
		case 7: printf("%-10s V%01X,V%01X,V%01X", "SUB.", code[0] & 0xf, code[1] >> 4); break; 


		}
	}
	case 0x09: printf("9 not handled yet"); break;

		// op code ANNN : moves an address to the memory address register I
	case 0x0a:
	{
		uint8_t addresshi = code[0] & 0x0f;
		printf("%-10s I,#$%01x02x", "MVI", addresshi, code[1]);

	}
	break;

	case 0x0b: printf("b not handles yet"); break;
	case 0x0c: printf("c not handles yet"); break;
	case 0x0d: printf("d not handles yet"); break;
	case 0x0e: printf("e not handles yet"); break;
	case 0x0f: printf("f not handles yet"); break;


	}
}



//main function to read in CHIP-8 programs
int main(int argc, char** argv)
{
	FILE* f = fopen(argv[1], "rb");
	if (f == NULL)
	{
		printf("error: Couldn't open %s\n", argv[1]);
		exit(1);
	}

	//Get the files size
	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);

	//CHIP-8 convention puts programs in memory at 0x200
	//They will all have hardcoded addresses expecting that
	//
	//Read the file into memory at 0x200 and close it

	unsigned char* buffer = (unsigned char*)malloc(fsize + 0x200);
	fread(buffer + 0x200, fsize, 1, f);
	fclose(f);

	//program counter set at 0x200 address
	int pc = 0x200;
	while (pc < fsize + 0x200) 
	{
	DisassembleChip8Op(buffer, pc);
	pc += 2;							//increment by 2 bytes
	printf("\n");

	}
	return 0;
}




