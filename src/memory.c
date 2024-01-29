#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "memory.h"

uint16_t memory[4096];

int memory_init(char *rom_name)
{
	// zero out memory
	memset(memory, 0, 4096);

	// load program into memory
	FILE *rom = fopen(rom_name, "rb");
	fseek(rom, 0, SEEK_END);
	size_t romsize = ftell(rom);
	rewind(rom);

	if (romsize > 4096) {
		fprintf(stderr, "ROM is larger than 4096 bytes!\n");
		return -1;
	}

	fread(memory, romsize, 1, rom);

	return 0;
}
