// SPDX-FileCopyrightText: 2024 Jozef Nagy <schkwve@gmail.com>
// SPDX-License-Identifier: MIT

#include <SDL2/SDL.h>

#include "chip8.h"
#include "input.h"

void input_get(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT: {
			g_machine.state = QUIT;
			break;
		case SDL_KEYDOWN: {
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE: {
				g_machine.state = QUIT;
				break;
			}
			case SDLK_F2: {
				if (g_machine.state == RUNNING) {
					SDL_SetWindowTitle(g_machine.win,
									   "CHIP-8 Emulator (Paused)");
					g_machine.state = PAUSED;
				} else {
					SDL_SetWindowTitle(g_machine.win, "CHIP-8 Emulator");
					g_machine.state = RUNNING;
				}
				break;
			}
			case SDLK_F5: {
				chip8_reboot();
				break;
			}
			case SDLK_RIGHTBRACKET: {
				if (g_machine.sound_volume < INT16_MAX) {
					g_machine.sound_volume += 250;
				}
				break;
			}
			case SDLK_LEFTBRACKET: {
				if (g_machine.sound_volume > 0) {
					g_machine.sound_volume -= 250;
				}
				break;
			}

			case SDLK_1:
				g_machine.keypad[0x1] = 1;
				break;
			case SDLK_2:
				g_machine.keypad[0x2] = 1;
				break;
			case SDLK_3:
				g_machine.keypad[0x3] = 1;
				break;
			case SDLK_4:
				g_machine.keypad[0xC] = 1;
				break;

			case SDLK_q:
				g_machine.keypad[0x4] = 1;
				break;
			case SDLK_w:
				g_machine.keypad[0x5] = 1;
				break;
			case SDLK_e:
				g_machine.keypad[0x6] = 1;
				break;
			case SDLK_r:
				g_machine.keypad[0xD] = 1;
				break;

			case SDLK_a:
				g_machine.keypad[0x7] = 1;
				break;
			case SDLK_s:
				g_machine.keypad[0x8] = 1;
				break;
			case SDLK_d:
				g_machine.keypad[0x9] = 1;
				break;
			case SDLK_f:
				g_machine.keypad[0xE] = 1;
				break;

			case SDLK_z:
				g_machine.keypad[0xA] = 1;
				break;
			case SDLK_x:
				g_machine.keypad[0x0] = 1;
				break;
			case SDLK_c:
				g_machine.keypad[0xB] = 1;
				break;
			case SDLK_v:
				g_machine.keypad[0xF] = 1;
				break;

			default:
				break;
			}
			break;
		}
		case SDL_KEYUP: {
			switch (event.key.keysym.sym) {
			case SDLK_1:
				g_machine.keypad[0x1] = 0;
				break;
			case SDLK_2:
				g_machine.keypad[0x2] = 0;
				break;
			case SDLK_3:
				g_machine.keypad[0x3] = 0;
				break;
			case SDLK_4:
				g_machine.keypad[0xC] = 0;
				break;

			case SDLK_q:
				g_machine.keypad[0x4] = 0;
				break;
			case SDLK_w:
				g_machine.keypad[0x5] = 0;
				break;
			case SDLK_e:
				g_machine.keypad[0x6] = 0;
				break;
			case SDLK_r:
				g_machine.keypad[0xD] = 0;
				break;

			case SDLK_a:
				g_machine.keypad[0x7] = 0;
				break;
			case SDLK_s:
				g_machine.keypad[0x8] = 0;
				break;
			case SDLK_d:
				g_machine.keypad[0x9] = 0;
				break;
			case SDLK_f:
				g_machine.keypad[0xE] = 0;
				break;

			case SDLK_z:
				g_machine.keypad[0xA] = 0;
				break;
			case SDLK_x:
				g_machine.keypad[0x0] = 0;
				break;
			case SDLK_c:
				g_machine.keypad[0xB] = 0;
				break;
			case SDLK_v:
				g_machine.keypad[0xF] = 0;
				break;
			}
			break;
		}
		default: {
			break;
		}
		}
		}
	}
}
