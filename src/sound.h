#ifndef SOUND_H
#define SOUND_H

int sound_init(void);

void sound_callback(void *unused, uint8_t *stream, int len);

#endif /* SOUND_H */
