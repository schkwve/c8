#include <stdio.h>
#include <stdbool.h>

#include "cpu.h"
#include "display.h"
#include "memory.h"

struct cpu cpu;

int cpu_init(void)
{
	// set sane defaults
	cpu.pc = 0x200;
	cpu.index = 0;
	cpu.sp = &cpu.stack[0];

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

	uint16_t ret = (g_memory[cpu.pc] << 8) | (g_memory[cpu.pc + 1]);
	cpu.pc += 2;
	return ret;
}

void cpu_execute(uint16_t instruction)
{
	uint16_t nnn = instruction & 0x0FFF;
	uint8_t nn = instruction & 0x00FF;
	uint8_t n = instruction & 0x000F;
	uint8_t x = (instruction >> 8) & 0x0F;
	uint8_t y = (instruction >> 4) & 0x0F;

	switch ((instruction >> 12) & 0x0F) {
		case 0x00: {
			if (nn == 0xE0) {
				display_clear_screen();
			} else if (nn == 0xEE) {
				cpu.pc = *--cpu.sp;
			}
			break;
		}
		case 0x01: {
			cpu.pc = nnn;
			break;
		}
		case 0x02: {
			*cpu.sp++ = cpu.pc;
			cpu.pc = nnn;
			break;
		}
		case 0x03: {
			if (cpu.v[x] == nn) {
				cpu.pc += 2;
			}
			break;
		}
		case 0x04: {
			if (cpu.v[x] != nn) {
				cpu.pc += 2;
			}
			break;
		}
		case 0x05: {
			if (cpu.v[x] == cpu.v[y]) {
				cpu.pc += 2;
			}
			break;
		}
		case 0x06: {
			cpu.v[x] = nn;
			break;
		}
		case 0x07: {
			cpu.v[x] += nn;
			break;
		}
		case 0x08: {
			switch (n) {
			case 0: {
				cpu.v[x] = cpu.v[y];
				break;
			}
			default: {
				break;
			}
			}
			break;
		}
		case 0x0A: {
			cpu.index = nnn;
			break;
		}
		case 0x0D: {
			uint8_t ren_x = cpu.v[x] % WINDOW_WIDTH;
            uint8_t ren_y = cpu.v[y] % WINDOW_HEIGHT;
            const uint8_t ox = ren_x;

            cpu.v[0x0F] = 0;

            for (uint8_t i = 0; i < n; i++) {
                const uint8_t sprite_data = g_memory[cpu.index + i];
                ren_x = ox;

                for (int8_t j = 7; j >= 0; j--) {
                    uint8_t *pixel = &g_framebuffer[ren_y * WINDOW_WIDTH + ren_x];
                    const uint8_t sprite_bit = (sprite_data & (1 << j)) ? 1 : 0;

                    if (sprite_bit && *pixel) {
                        cpu.v[0x0F] = 1;
                    }

                    *pixel ^= sprite_bit;

                    if (++ren_x >= WINDOW_WIDTH) break;
                }

                if (++ren_y >= WINDOW_HEIGHT) break;
            }
			break;
        }
		default: {
			fprintf(stderr, "Instruction '%x' not implemented yet!\n", instruction);
			break;
		}
	}
}
