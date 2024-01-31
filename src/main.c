#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>

#include "cpu.h"
#include "display.h"
#include "memory.h"

int main(int argc, char *argv[])
{
	SDL_Event event;

	// initialize display
	if (display_init() != 0) {
		fprintf(stderr, "Failed to initialize display!\n");
		SDL_DestroyRenderer(g_ren);
		SDL_DestroyWindow(g_win);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	// initialize cpu
	if (cpu_init() != 0) {
		fprintf(stderr, "Failed to initialize CPU!\n");
		SDL_DestroyRenderer(g_ren);
		SDL_DestroyWindow(g_win);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	// initialize memory
	if (memory_init(argv[1]) != 0) {
		fprintf(stderr, "Failed to initialize memory!\n");
		SDL_DestroyRenderer(g_ren);
		SDL_DestroyWindow(g_win);
		SDL_Quit();
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

	SDL_DestroyRenderer(g_ren);
	SDL_DestroyWindow(g_win);
	SDL_Quit();

	return EXIT_SUCCESS;
}
