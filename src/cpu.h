#ifndef CPU_H
#define CPU_H

#include <stdint.h>

struct cpu {
	uint16_t pc;
	uint16_t index;

	uint8_t delay_timer;
	uint8_t sound_timer;

	uint8_t v[16];
};

extern struct cpu cpu;

int cpu_init(void);

uint16_t cpu_fetch(void);
void cpu_execute(uint16_t instruction);

#endif /* CPU_H */
