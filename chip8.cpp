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

	unsigned char* buffer = malloc(fsize + 0x200);
	fread(buffer + 0x200, fsize, 1, f);
	fclose(f);

	//program counter set at 0x200 address
	int pc = 0x200;
	while (pc < fsize + 0x200) )
	{
	DisassembleChip8Op(buffer, pc);
	pc += 2;							//increment by 2 bytes
	printf("\n");

	}
	return 0;
}



//function to process op codes
void DisassembleChip8Op(uint8_t* codebuffer, int pc)
{
	uint8_t* code = &codebuffer[pc];	//??
	unint8_t firstnib = (code[0] >> 4); //upper 4 bits of the first byte (opcode)

	printf("%04x %02x %02x ", pc, code[0], code[1]);

	switch (firstnib)
	{
		case 0x00: printf("0 not handled yet"); break;
		case 0x01: printf("1 not handled yet"); break;
		case 0x02: printf("2 not handled yet"); break;
		case 0x03: printf("3 not handled yet"); break;
		case 0x04: printf("4 not handled yet"); break;
		case 0x05: printf("5 not handled yet"); break;

		// op code 6XNN movesan immediate value to a register
		case 0x06:
		{
			uint8_t reg = code[0] & 0x0f;
			printf("%-10s V%01X,#$%02x", "MVI", reg, code[1]);
		}
		break;
		case 0x07: printf("7 not handled yet"); break;
		case 0x08: printf("8 not handled yet"); break;
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