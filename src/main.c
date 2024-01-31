#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chip8.h"
#include "cpu.h"
#include "display.h"
#include "memory.h"
#include "input.h"

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
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	// initialize display
	if (display_init() != 0) {
		fprintf(stderr, "Failed to initialize display!\n");
		return EXIT_FAILURE;
	}

	// initialize cpu
	if (cpu_init() != 0) {
		fprintf(stderr, "Failed to initialize CPU!\n");
		return EXIT_FAILURE;
	}

	// initialize memory
	if (memory_init(argv[1]) != 0) {
		fprintf(stderr, "Failed to initialize memory!\n");
		return EXIT_FAILURE;
	}

	// we are running!
	g_machine.state = RUNNING;

	// main loop
    while (g_machine.state != QUIT) {
		const uint64_t start_time = SDL_GetTicks();

		input_get();
		if (g_machine.state == PAUSED) {
			continue;
		}
        
		cpu_execute(cpu_fetch());
        display_update();

		if (g_machine.cpu.delay_timer > 0) {
			g_machine.cpu.delay_timer--;
		}
		if (g_machine.cpu.sound_timer > 0) {
			// TODO: play a beep
			g_machine.cpu.sound_timer--;
		} else {
			// TODO: stop beeping
		}

		const uint64_t end_time = SDL_GetTicks() - start_time;

		// ~700 instructions per second
		struct timespec sleep_time = {0, 15000000 - end_time};
		nanosleep(&sleep_time, NULL);
    }

	return EXIT_SUCCESS;
}

void cleanup_and_die(void)
{
	if (g_machine.ren != NULL) {
		SDL_DestroyRenderer(g_machine.ren);
	}

	if (g_machine.win != NULL) {
		SDL_DestroyWindow(g_machine.win);
	}

	SDL_Quit();
}
