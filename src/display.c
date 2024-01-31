#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "chip8.h"
#include "cpu.h"
#include "display.h"

int display_init(void)
{
	g_machine.win = SDL_CreateWindow("CHIP-8 Emulator", 100, 100, WINDOW_WIDTH * WINDOW_SCALE_FACTOR, WINDOW_HEIGHT * WINDOW_SCALE_FACTOR, SDL_WINDOW_SHOWN);
	if (g_machine.win == NULL) {
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	g_machine.ren = SDL_CreateRenderer(g_machine.win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (g_machine.ren == NULL) {
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		return -1;
	}

	// display nothing
	SDL_SetRenderDrawColor(g_machine.ren, 0, 0, 0, 0);
	SDL_RenderClear(g_machine.ren);
	SDL_RenderPresent(g_machine.ren);

	memset(g_machine.framebuffer, 0, sizeof(g_machine.framebuffer));

	return 0;
}

void display_clear_screen(void)
{
	memset(g_machine.framebuffer, 0, sizeof(g_machine.framebuffer));
}

void display_update(void)
{
	// last two parameters are the scale factor (for width and height, respectively)
    SDL_Rect pixel = {0, 0, WINDOW_SCALE_FACTOR, WINDOW_SCALE_FACTOR};

    for (uint32_t i = 0; i < sizeof(g_machine.framebuffer); i++) {
        pixel.x = (i % 64) * WINDOW_SCALE_FACTOR;
        pixel.y = (i / 64) * WINDOW_SCALE_FACTOR;

        if (g_machine.framebuffer[i] == 1) {
            SDL_SetRenderDrawColor(g_machine.ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(g_machine.ren, &pixel);
        } else if (g_machine.framebuffer[i] == 0) {
            SDL_SetRenderDrawColor(g_machine.ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(g_machine.ren, &pixel);
        } else {
			// TODO: crash the program
			// printf("Framebuffer contains wrong information!\n");
		}
    }
    
    SDL_RenderPresent(g_machine.ren);
}
