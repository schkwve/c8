// SPDX-FileCopyrightText: 2024 Jozef Nagy <schkwve@gmail.com>
// SPDX-License-Identifier: MIT

#include <SDL2/SDL.h>

#include "chip8.h"
#include "sound.h"

int sound_init(void)
{
	SDL_AudioSpec request = { .freq = 44100,
							  .format = AUDIO_S16LSB,
							  .channels = 1,
							  .samples = 512,
							  .callback = sound_callback,
							  .userdata = NULL };

	SDL_AudioSpec response = { 0 };

	g_machine.sound_volume = 500;

	g_machine.sound_dev = SDL_OpenAudioDevice(NULL, 0, &request, &response, 0);
	if (g_machine.sound_dev == 0) {
		fprintf(stderr, "Couldn't get an audio device!\n");
		return -1;
	}

	if ((request.format != response.format) ||
		(request.channels != response.channels)) {
		fprintf(stderr, "Couldn't get desirable audio device!\n");
		return -1;
	}

	return 0;
}

void sound_callback(void *unused, uint8_t *stream, int len)
{
	int16_t *audio = (int16_t *)stream;
	static uint32_t running_sample_index = 0;
	const int32_t square_wave_period = 44100 / 440; // sample rate / frequency
	const int32_t half_square_wave_period = square_wave_period / 2;

	for (int i = 0; i < len / 2; i++) {
		if ((running_sample_index++ / half_square_wave_period) % 2) {
			audio[i] = g_machine.sound_volume;
		} else {
			audio[i] = -g_machine.sound_volume;
		}
	}
}
