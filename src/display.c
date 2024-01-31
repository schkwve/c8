#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "display.h"

SDL_Window *g_win = NULL;
SDL_Renderer *g_ren = NULL;
SDL_Texture *g_tex = NULL;

// values for the framebuffer can *only* be either 1 or 0.
// TODO: any other value should crash the program.
uint8_t g_framebuffer[64 * 32] = {0};

int display_init(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	g_win = SDL_CreateWindow("CHIP-8 Emulator", 100, 100, WINDOW_WIDTH * 10, WINDOW_HEIGHT * 10, SDL_WINDOW_SHOWN);
	if (g_win == NULL) {
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	g_ren = SDL_CreateRenderer(g_win, -1, SDL_RENDERER_ACCELERATED);
	if (g_ren == NULL) {
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		return -1;
	}

	// display nothing
	SDL_SetRenderDrawColor(g_ren, 0, 0, 0, 0);
	SDL_RenderClear(g_ren);
	SDL_RenderPresent(g_ren);

	memset(g_framebuffer, 0, sizeof(g_framebuffer));

	return 0;
}

void display_clear_screen(void)
{
	memset(g_framebuffer, 0, sizeof(g_framebuffer));
}

void display_update(void)
{
	// last two parameters are the scale factor (for width and height, respectively)
    SDL_Rect pixel = {0, 0, 10, 10};

    for (uint32_t i = 0; i < sizeof(g_framebuffer); i++) {
        pixel.x = (i % 64) * 10;
        pixel.y = (i / 64) * 10;

        if (g_framebuffer[i] == 1) {
            SDL_SetRenderDrawColor(g_ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(g_ren, &pixel);
        } else if (g_framebuffer[i] == 0) {
            SDL_SetRenderDrawColor(g_ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(g_ren, &pixel);
        } else {
			// TODO: crash the program
			// printf("Framebuffer contains wrong information!\n");
		}
    }
    
    SDL_RenderPresent(g_ren);
}
