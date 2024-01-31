#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#include "cpu.h"
#include "display.h"
#include "memory.h"

enum machine_state {
	RUNNING,
	PAUSED,
	QUIT
};

typedef struct {
	// general
	enum machine_state state;

	// display
	SDL_Window *win;
	SDL_Renderer *ren;
	// values for the framebuffer can *only* be either 1 or 0.
	// TODO: any other value should crash the program.
	uint8_t framebuffer[64 * 32];
	uint8_t should_render;

	// memory
	uint8_t memory[4096];

	// cpu
	struct cpu cpu;

	// input
	uint8_t keypad[16];

	// sound
	uint16_t sound_volume;
	SDL_AudioSpec sound_spec;
	SDL_AudioDeviceID sound_dev;
} machine_t;

extern machine_t g_machine;

void cleanup_and_die(void);

#endif /* CHIP8_H */
