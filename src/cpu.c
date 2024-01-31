#include <stdio.h>
#include <stdbool.h>

#include "chip8.h"
#include "cpu.h"
#include "display.h"
#include "memory.h"

int cpu_init(void)
{
	// set sane defaults
	g_machine.cpu.pc = 0x200;
	g_machine.cpu.index = 0;
	g_machine.cpu.sp = &g_machine.cpu.stack[0];

	g_machine.cpu.delay_timer = 60;
	g_machine.cpu.sound_timer = 60;

	for (int i = 0; i < 16; i++) {
		g_machine.cpu.v[i] = 0;
	}

	return 0;
}

uint16_t cpu_fetch(void)
{
	if (g_machine.cpu.pc > 4095) {
		g_machine.cpu.pc = 0;
	}

	uint16_t ret = (g_machine.memory[g_machine.cpu.pc] << 8) | (g_machine.memory[g_machine.cpu.pc + 1]);
	g_machine.cpu.pc += 2;
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
			switch (nn) {
			case 0xE0:
				display_clear_screen();
				break;
			case 0xEE:
				g_machine.cpu.pc = *--g_machine.cpu.sp;
				break;
			default:
				fprintf(stderr, "Instruction '%x' not implemented yet!\n", instruction);
				break;
			}
			break;
		}
		case 0x01: {
			g_machine.cpu.pc = nnn;
			break;
		}
		case 0x02: {
			*g_machine.cpu.sp++ = g_machine.cpu.pc;
			g_machine.cpu.pc = nnn;
			break;
		}
		case 0x03: {
			if (g_machine.cpu.v[x] == nn) {
				g_machine.cpu.pc += 2;
			}
			break;
		}
		case 0x04: {
			if (g_machine.cpu.v[x] != nn) {
				g_machine.cpu.pc += 2;
			}
			break;
		}
		case 0x05: {
			if (g_machine.cpu.v[x] == g_machine.cpu.v[y]) {
				g_machine.cpu.pc += 2;
			}
			break;
		}
		case 0x06: {
			g_machine.cpu.v[x] = nn;
			break;
		}
		case 0x07: {
			g_machine.cpu.v[x] += nn;
			break;
		}
		case 0x08: {
			switch (n) {
			case 0x00: {
				g_machine.cpu.v[x] = g_machine.cpu.v[y];
				break;
			}
			case 0x01: {
				g_machine.cpu.v[0x0F] = 0;
				g_machine.cpu.v[x] |= g_machine.cpu.v[y];
				break;
			}
			case 0x02: {
				g_machine.cpu.v[0x0F] = 0;
				g_machine.cpu.v[x] &= g_machine.cpu.v[y];
				break;
			}
			case 0x03: {
				g_machine.cpu.v[0x0F] = 0;
				g_machine.cpu.v[x] ^= g_machine.cpu.v[y];
				break;
			}
			case 0x04: {
				g_machine.cpu.v[0x0F] = ((uint16_t)(g_machine.cpu.v[x] + g_machine.cpu.v[y]) > 255) ? 1 : 0;
				g_machine.cpu.v[x] += g_machine.cpu.v[y];
				break;
			}
			case 0x05: {
				g_machine.cpu.v[0x0F] = (g_machine.cpu.v[y] <= g_machine.cpu.v[x]) ? 1 : 0;
				g_machine.cpu.v[x] -= g_machine.cpu.v[y];
				break;
			}
			case 0x06: {
				g_machine.cpu.v[0x0F] = g_machine.cpu.v[y] & 1;
				g_machine.cpu.v[x] = g_machine.cpu.v[y] >> 1;
				break;
			}
			case 0x07: {
				g_machine.cpu.v[0x0F] = g_machine.cpu.v[y] & 1;
				g_machine.cpu.v[x] = g_machine.cpu.v[y] - g_machine.cpu.v[x];
				break;
			}
			case 0x0E: {
				g_machine.cpu.v[0x0F] = (g_machine.cpu.v[y] & 0x80) >> 7;
				g_machine.cpu.v[x] = g_machine.cpu.v[y] << 1;
				break;
			}
			default: {
				fprintf(stderr, "Instruction '%x' not implemented yet!\n", instruction);
				break;
			}
			}
			break;
		}
		case 0x09: {
			if (g_machine.cpu.v[x] != g_machine.cpu.v[y]) {
				g_machine.cpu.pc += 2;
			}
			break;
		}
		case 0x0A: {
			g_machine.cpu.index = nnn;
			break;
		}
		case 0x0B: {
			g_machine.cpu.pc = g_machine.cpu.v[0x00] + nnn;
			break;
		}
		case 0x0C: {
			g_machine.cpu.v[x] = (rand() % 256) & nn;
			break;
		}
		case 0x0D: {
			uint8_t ren_x = g_machine.cpu.v[x] % WINDOW_WIDTH;
            uint8_t ren_y = g_machine.cpu.v[y] % WINDOW_HEIGHT;
            const uint8_t ox = ren_x;

            g_machine.cpu.v[0x0F] = 0;

            for (uint8_t i = 0; i < n; i++) {
                const uint8_t sprite_data = g_machine.memory[g_machine.cpu.index + i];
                ren_x = ox;

                for (int8_t j = 7; j >= 0; j--) {
                    uint8_t *pixel = &g_machine.framebuffer[ren_y * WINDOW_WIDTH + ren_x];
                    const uint8_t sprite_bit = (sprite_data & (1 << j)) ? 1 : 0;

                    if (sprite_bit && *pixel) {
                        g_machine.cpu.v[0x0F] = 1;
                    }

                    *pixel ^= sprite_bit;

                    if (++ren_x >= WINDOW_WIDTH) break;
                }

                if (++ren_y >= WINDOW_HEIGHT) break;
            }
			break;
        }
		case 0x0E: {
			switch (nn) {
			case 0x9E: {
				if (g_machine.keypad[g_machine.cpu.v[x]] == 1) {
					g_machine.cpu.pc += 2;
				}
				break;
			}
			case 0xA1: {
				if (g_machine.keypad[g_machine.cpu.v[x]] != 1) {
					g_machine.cpu.pc += 2;
				}
				break;
			}
			default: {
				fprintf(stderr, "Instruction '%x' not implemented yet!\n", instruction);
				break;
			}
			}
			break;
		}
		case 0x0F: {
			switch (nn) {
			case 0x07: {
				g_machine.cpu.v[x] = g_machine.cpu.delay_timer;
				break;
			}
			case 0x0A: {
				static bool pressed = 0;
				static uint8_t key = 0xFF;

				for (uint8_t i = 0; i < sizeof(g_machine.keypad) && key == 0xFF; i++) {
					if (g_machine.keypad[i] == 1) {
						key = i;
						pressed = 1;
						break;
					}
				}

				if (pressed == 1) {
					// wait until release
					if (g_machine.keypad[key] == 0) {
						g_machine.cpu.v[x] = key;
						key = 0xFF;
						pressed = 0;
					} else {
						g_machine.cpu.pc -= 2;
					}
				} else {
					g_machine.cpu.pc -= 2;
				}
				break;
			}
			case 0x15: {
				g_machine.cpu.delay_timer = g_machine.cpu.v[x];
				break;
			}
			case 0x18: {
				g_machine.cpu.sound_timer = g_machine.cpu.v[x];
				break;
			}
			case 0x1E: {
				g_machine.cpu.index += g_machine.cpu.v[x];
				break;
			}
			case 0x29: {
				g_machine.cpu.index = g_machine.memory[g_machine.cpu.index + g_machine.cpu.v[x]];
				break;
			}
			case 0x33: {
				int num = g_machine.cpu.v[x];
				g_machine.memory[g_machine.cpu.index + 2] = num % 10;
				num /= 10;
				g_machine.memory[g_machine.cpu.index + 1] = num % 10;
				num /= 10;
				g_machine.memory[g_machine.cpu.index] = num;
				break;
			}
			case 0x55: {
				for (int i = 0; i < x; i++) {
					g_machine.memory[g_machine.cpu.index + i] = g_machine.cpu.v[i];
				}
				break;
			}
			case 0x65: {
				for (int i = 0; i < x; i++) {
					g_machine.cpu.v[i] = g_machine.memory[g_machine.cpu.index + i];
				}
				break;
			}
			default: {
				fprintf(stderr, "Instruction '%x' not implemented yet!\n", instruction);
				break;
			}
			}
			break;
		}
		default: {
			fprintf(stderr, "Instruction '%x' not implemented yet!\n", instruction);
			break;
		}
	}
}
