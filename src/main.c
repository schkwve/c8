// SPDX-FileCopyrightText: 2024 Jozef Nagy <schkwve@gmail.com>
// SPDX-License-Identifier: MIT

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chip8.h"
#include "cpu.h"
#include "display.h"
#include "memory.h"
#include "input.h"
#include "sound.h"

machine_t g_machine;

int main(int argc, char *argv[])
{
	// register an exit function
	// to cleanup
	atexit(cleanup_and_die);

	// initialize a random timer
	time_t t;
	srand((unsigned)time(&t));

	// startup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	// initialize display
	if (display_init() != 0) {
		fprintf(stderr, "Failed to initialize display!\n");
		return EXIT_FAILURE;
	}

	// initialize sound
	if (sound_init() != 0) {
		fprintf(stderr, "Failed to initialize sound!\n");
		return EXIT_FAILURE;
	}

	// initialize cpu
	if (cpu_init() != 0) {
		fprintf(stderr, "Failed to initialize CPU!\n");
		return EXIT_FAILURE;
	}

	// initialize memory
	g_machine.rompath = argv[1];
	if (memory_init() != 0) {
		fprintf(stderr, "Failed to initialize memory!\n");
		return EXIT_FAILURE;
	}

	// we are running!
	g_machine.state = RUNNING;
	g_machine.should_render = 0;

	// main loop
    while (g_machine.state != QUIT) {

		input_get();
		if (g_machine.state == PAUSED) {
			continue;
		}

		const uint64_t start_time = SDL_GetPerformanceCounter();
        
		for (uint32_t i = 0; i < (700 / 60); i++) {
			cpu_execute(cpu_fetch());
		}

		const uint64_t end_time = SDL_GetPerformanceCounter();
		const double elapsed_time = (double)((end_time - start_time) * 1000) / SDL_GetPerformanceFrequency();

		SDL_Delay(16.67f > elapsed_time ? 16.67f - elapsed_time : 0);

		if (g_machine.should_render == 1) {
			g_machine.should_render = 0;
        	display_update();
		}

		if (g_machine.cpu.delay_timer > 0) {
			g_machine.cpu.delay_timer--;
		}
		if (g_machine.cpu.sound_timer > 0) {
			SDL_PauseAudioDevice(g_machine.sound_dev, 0);
			g_machine.cpu.sound_timer--;
		} else {
			SDL_PauseAudioDevice(g_machine.sound_dev, 1);
		}
    }

	return EXIT_SUCCESS;
}

void chip8_reboot(void)
{
	// reset display
	memset(g_machine.framebuffer, 0, sizeof(g_machine.framebuffer));
	display_update();

	// reset cpu
	memset(&g_machine.cpu, 0, sizeof(g_machine.cpu));
	if (cpu_init() != 0) {
		fprintf(stderr, "Failed to initialize CPU!\n");
		exit(-1);
	}

	// reset memory
	memset(g_machine.memory, 0, sizeof(g_machine.memory));
	if (memory_init() != 0) {
		fprintf(stderr, "Failed to initialize memory!\n");
		exit(-1);
	}
}

void cleanup_and_die(void)
{
	if (g_machine.sound_dev != 0) {
		SDL_CloseAudioDevice(g_machine.sound_dev);
	}

	if (g_machine.ren != NULL) {
		SDL_DestroyRenderer(g_machine.ren);
	}

	if (g_machine.win != NULL) {
		SDL_DestroyWindow(g_machine.win);
	}

	SDL_Quit();
}
