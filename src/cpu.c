#include <stdio.h>

#include "cpu.h"
#include "display.h"
#include "memory.h"

struct cpu cpu;

int cpu_init(void)
{
	// set sane defaults
	cpu.pc = 0;
	cpu.index = 0;

	cpu.delay_timer = 60;
	cpu.sound_timer = 60;

	for (int i = 0; i < 16; i++) {
		cpu.v[i] = 0;
	}

	return 0;
}

uint16_t cpu_fetch(void)
{
	if (cpu.pc > 4095) {
		cpu.pc = 0;
	}

	uint16_t ret = memory[cpu.pc++];
	return (ret >> 8) | (ret << 8);;
}

void cpu_execute(uint16_t instruction)
{
	switch (instruction >> 12) {
		case 0x00: {
			if (instruction == 0x00E0) {
				display_clear_screen();
			} else if (instruction == 0x00EE) {
				// cpu_return();
			}
			break;
		}
		case 0x01: {
			uint8_t addr = (instruction & 0x0FFF);
			cpu.pc = addr;
			break;
		}
		case 0x06: {
			uint8_t reg = (instruction & 0x0F00) >> 8;
			cpu.v[reg] = instruction & 0xFF;
			break;
		}
		case 0x07: {
			uint8_t reg = (instruction & 0x0F00) >> 8;
			cpu.v[reg] += instruction & 0xFF;
			break;
		}
		case 0x08: {
			switch (instruction & 0x0F) {
			case 0: {
				uint8_t a = (instruction & 0x0F00) >> 8;
				uint8_t b = (instruction & 0x00F0) >> 4;
				cpu.v[a] = cpu.v[b];
				break;
			}
			default: {
				break;
			}
			}
			uint8_t reg = (instruction & 0x0F00) >> 8;
			cpu.v[reg] += instruction & 0xFF;
			break;
		}
		case 0x0A: {
			uint8_t addr = (instruction & 0x0FFF);
			cpu.index = addr;
			break;
		}
		case 0x0D: {
			uint8_t x = (instruction & 0x0F00) >> 8;
			uint8_t y = (instruction & 0x00F0) >> 4;
			uint8_t height = instruction & 0x0F;
			display_draw(x, y, height);
			break;
		}
		default: {
			// fprintf(stderr, "Instruction '%x' not implemented yet!\n", instruction);
			break;
		}
	}
}
