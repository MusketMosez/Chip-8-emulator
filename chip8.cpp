#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


//function to process op codes
static int DisassembleChip8Op(uint8_t* codebuffer, int pc)
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
		case 7: printf("%-10s V%01X,V%01X,V%01X", "SUB.", code[0] & 0xf, code[1] >> 4); break; // Sets VX to VY minus VX, VF is set to 0 when there's a borrow
		case 0xe: printf("%-10s V%01X,V%01X", "SHL.", code[0] & 0xf, code[1] >> 4); break; //Stores MSB of VX in VF then shift VX to right by 1




		}
	}
	case 0x09: printf("%-10s V%01X,V%01X", "SKIP.NE", code[0]&0xf, code[1]>>4); break; //Skips next instruction if VX != VY


		// op code ANNN : moves an address to the memory address register I
	case 0x0a:
	{
		uint8_t addresshi = code[0] & 0x0f;
		printf("%-10s I,#$%01x02x", "MVI", addresshi, code[1]);

	}
	break;

	case 0x0b: printf("%-10s $%01x%02x(V0)", "JUMP", code[0]&0xf, code[1]); break; //Jumps to the address NNN plus V0
	case 0x0c: printf("%-10s V%01X,#$%02X", "RNDMSK", code[0]&0xf, code[1]); break;//Sets VX=rand()&NN
	case 0x0d: printf("%-10s V%01X,V%01X,#$%01x", "SPRITE", code[0]&0xf, code); break;//Draws sprite
	
	//Operations representing key actions (interrupts?)
	case 0x0e: 
		switch (code[1])
		{
		case 0x9E: printf("%-10s V%01X", "SKIPKEY.Y", code[0] & 0xf); break; //Skips the next  instruction if the key stored in VX is pressed
		case 0xA1: printf("%-10s V%01X", "SKIPKEY.N", code[0] & 0xf); break; //Skips next instruction if the key stored in VX is not pressed

		}


		break;
	
	//Timers? and some memory operations
	case 0x0f: 
		switch (code[1])
		{
		case 0x07: printf("%-10s V%01X,DELAY", "MOV", code[0] & 0xf); break;   //Set VX to value of delay timer
		case 0x0a: printf("%-10s V%01X", "KEY", code[0] & 0xf); break;         //Wait for key press, then store key in VX
		case 0x15: printf("%-10s DELAY,V%01X", "MOV", code[0] & 0xf); break;   //Set delay time to VX
		case 0x18: printf("%-10s SOUND,V%01X", "MOV", code[0] & 0xf); break;   //Set sound timer to VX
		case 0x1e: printf("%-10s I,V%01X", "ADI", code[0] & 0xf); break;	   //Adds VX to I
		case 0x29: printf("%-10s I,V%01X", "SPRITECHAR", code[0] & 0xf); break;//Sets I to the location of the sprite for the character in VX
		case 0x33: printf("%-10s (I),V%01X", "MOVBCD", code[0] & 0xf); break;  //Gobblety gook
		case 0x55: printf("%-10s (I),V0-V%01X", "MOVM", code[0] & 0xf); break; //Stores V0 to VX (including VX) in memory starting at address I
		case 0x65: printf("%-10s V0-V%01X,(I)", "MOVM", code[0] & 0xf); break; //Fills V0 to VX (including VX) with values from memory starting at address I
		default: printf("UNKNOWN F"); break;


		}
		break;


	}
	return 2;
}

//struct to handle state of the CHIP-8
typefef struct Chip8State {
	uint8_t		V[16];	//16 8-bit registers
	uint16_t	I;		//Memory address register
	uint16_t	SP;		//Stack pointer
	uint16_t	PC;		//Program counter
	uint8_t		delay;	//delay timer
	uint8_t		sound;  //sound timer
	uint8_t		*memory 
	uint8_t		*screen //this is at memory[0xF00]
} Chip8State;

//initialise CHIP-8 state
Chip8State* InitChip8(void)
{
	CHip8State* s = calloc(sizeof(Chip8State), 1);

	s->memory = calloc(1024 * 4, 1);
	s->screen = &s->memory[0xf00];
	s->SP = 0xfa0;
	s->PC = 0x200;

	return s;
}

//main emulation routine
void EmulateChip8Op(Chip8State* state)
{
	uint8_t* op = &state->memory[state->PC]; //fetch next instruction from PC

	int highnib = (*op & 0xf0) >> 4;
	switch (highnib)
	{
	case 0x00: UnimplementedInstruction(state); break;
	case 0x01:							//Jump to address NNN
	{
		uint16_t target = ((code[0]) & 0xf) << 8) | code[1]; //set target to address NNN
		state->PC = target;		//goto NNN
	}
	case 0x02: UnimplementedInstruction(state); break;
	case 0x03:							//Skip next if VX = NN
	{
		uint8_t reg = code[0] & 0xf; //assign register VX
		if (state->V[reg] == code[1]) //if VX = NN
			state->PC += 2;
		state->PC += 2;

	}
	break;
	case 0x04: UnimplementedInstruction(state); break;
	case 0x05: UnimplementedInstruction(state); break;
	case 0x06:						//MOV VX,#$NN, Set VX =NN
	{
		uint8_t reg = code[0] & 0xf; //set reg to VX
		state->V[reg] = code[1];	//VX = NN
		state->PC += 2;
	}
	case 0x07: UnimplementedInstruction(state); break;
	case 0x08: UnimplementedInstruction(state); break;
	case 0x09: UnimplementedInstruction(state); break;
	case 0x0a:						//MOV I, #$NNN, set I = NNN
	{
		state->I = ((code[0 & 0xf]) << 8) | code[1];
		state->PC += 2;
	}
	break;
	case 0x0b: UnimplementedInstruction(state); break;
	case 0x0c: UnimplementedInstruction(state); break;
	case 0x0e: UnimplementedInstruction(state); break;
	case 0x0f: UnimplementedInstruction(state); break;

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




