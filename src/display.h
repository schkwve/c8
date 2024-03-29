// SPDX-FileCopyrightText: 2024 Jozef Nagy <schkwve@gmail.com>
// SPDX-License-Identifier: MIT

#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>

#define WINDOW_WIDTH 64
#define WINDOW_HEIGHT 32
#define WINDOW_SCALE_FACTOR 20

extern SDL_Window *g_win;
extern SDL_Renderer *g_ren;

extern uint8_t g_framebuffer[];

int display_init(void);

void display_clear_screen(void);
void display_update(void);

#endif /* DISPLAY_H */
