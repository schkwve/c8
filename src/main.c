#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chip8.h"
#include "cpu.h"
#include "display.h"
#include "memory.h"

int main(int argc, char *argv[])
{
	SDL_Event event;

	// register an exit function
	// to cleanup
	atexit(cleanup_and_die);

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

	// ~700 instructions per second
	struct timespec sleep_time = {0, 1428571};

	// main loop
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
		
		cpu_execute(cpu_fetch());

		// update window
        display_update();

		nanosleep(&sleep_time, NULL);
    }

	return EXIT_SUCCESS;
}

void cleanup_and_die(void)
{
	if (g_ren != NULL) {
		SDL_DestroyRenderer(g_ren);
	}

	if (g_win != NULL) {
		SDL_DestroyWindow(g_win);
	}

	SDL_Quit();
}
