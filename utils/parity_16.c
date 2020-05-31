#include <stdint.h>
#include "common.h"
#include "../utils.h"

uint8_t parity_16(uint16_t value) {
	return pop_count_16(value) & 0x01;

	/*
	(void)value;

	__asm
	__endasm;
	*/
}
