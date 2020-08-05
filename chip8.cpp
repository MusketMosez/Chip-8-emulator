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

		// 0x6XNN movesan immediate value to a register
		case 0x06:
		{
			uint8_t reg = code[0] & 0x0f;
			printf("%-10s V%01X,#$%02x", "MVI", reg, code[1]);
		}
		break;
		case 0x07: printf("7 not handled yet"); break;
	}
}