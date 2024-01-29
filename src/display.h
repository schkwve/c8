#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>

extern SDL_Window *g_win;
extern SDL_Renderer *g_ren;
extern SDL_Texture *g_tex;

int display_init(void);

void display_clear_screen(void);
void display_draw(uint8_t x, uint8_t y, uint8_t height);

#endif /* DISPLAY_H */
