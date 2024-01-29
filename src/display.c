#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "display.h"

SDL_Window *g_win = NULL;
SDL_Renderer *g_ren = NULL;
SDL_Texture *g_tex = NULL;

uint8_t g_framebuffer[640 * 320 * 4] = {0};

int display_init(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	g_win = SDL_CreateWindow("CHIP-8 Emulator", 100, 100, 640, 320, SDL_WINDOW_SHOWN);
	if (g_win == NULL) {
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	g_ren = SDL_CreateRenderer(g_win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (g_ren == NULL) {
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		return -1;
	}

	SDL_RenderSetLogicalSize(g_ren, 640, 320);

	g_tex = SDL_CreateTexture(g_ren, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 640, 320);
	if (g_tex == NULL) {
		fprintf(stderr, "SDL_CreateTexture Error: %s\n", SDL_GetError());
		return -1;
	}

	// display nothing
	SDL_SetRenderDrawColor(g_ren, 0, 0, 0, 0);
	SDL_RenderClear(g_ren);
	SDL_RenderPresent(g_ren);

	return 0;
}

void display_clear_screen(void)
{
	SDL_SetRenderDrawColor(g_ren, 0, 0, 0, 0);
	SDL_RenderClear(g_ren);
	SDL_RenderPresent(g_ren);
}

void display_draw(uint8_t x, uint8_t y, uint8_t height)
{
	// this does not work for some reason

    for (int render_y = 0; render_y < y + height; render_y++) {
		for (int render_x = 0; render_x < 8; render_x++) {
			uint8_t loc = 4 * (render_x + x) + render_y + (y + height);
			if (g_framebuffer[loc] == 255) {
				g_framebuffer[loc] = 0;
				cpu.v[0x0F] = 1;
			} else {
				g_framebuffer[loc] = 255;
			}
		}
	}

    // update texture with new data
    int texture_pitch = 0;
    void* texture_pixels = NULL;
    if (SDL_LockTexture(g_tex, NULL, &texture_pixels, &texture_pitch) != 0) {
        SDL_Log("Unable to lock texture: %s", SDL_GetError());
    }
    else {
        memcpy(texture_pixels, g_framebuffer, texture_pitch * 320);
    }
    SDL_UnlockTexture(g_tex);
}
