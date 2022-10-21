/*******************************************************************************
 *
 * ref.c - Reference implementations
 *
 * Copyright (c) 2022 Basil Hussain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ******************************************************************************/

#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "ref.h"

uint8_t swap_ref(const uint8_t value) {
	return ((value & 0xF0) >> 4) | ((value & 0x0F) << 4);
}

uint16_t bswap_16_ref(const uint16_t value) {
	return ((value & 0xFF00) >> 8) | ((value & 0x00FF) << 8);
}

uint32_t bswap_32_ref(const uint32_t value) {
	return ((value & 0x000000FFUL) << 24) | ((value & 0x0000FF00UL) << 8) | ((value & 0x00FF0000UL) >> 8) | ((value & 0xFF000000UL) >> 24);
}

uint8_t ctz_8_ref(uint8_t value) {
	uint8_t c = 0;
	if(value) {
		// Set value's trailing 0s to 1s and zero the rest.
		value = (value ^ (value - 1)) >> 1;
		while(value) {
			value >>= 1;
			c++;
		}
	} else {
		c = CHAR_BIT * sizeof(value);
	}
	return c;
}

uint8_t ctz_16_ref(uint16_t value) {
	uint8_t c = 0;
	if(value) {
		// Set value's trailing 0s to 1s and zero the rest.
		value = (value ^ (value - 1)) >> 1;
		while(value) {
			value >>= 1;
			c++;
		}
	} else {
		c = CHAR_BIT * sizeof(value);
	}
	return c;
}

uint8_t ctz_32_ref(uint32_t value) {
	uint8_t c = 0;
	if(value) {
		// Set value's trailing 0s to 1s and zero the rest.
		value = (value ^ (value - 1)) >> 1;
		while(value) {
			value >>= 1;
			c++;
		}
	} else {
		c = CHAR_BIT * sizeof(value);
	}
	return c;
}

uint8_t clz_8_ref(uint8_t value) {
	uint8_t c = 0;
	if(value) {
		uint8_t mask = 1U << ((CHAR_BIT * sizeof(value)) - 1);
		while(!(value & mask)) {
			mask >>= 1;
			c++;
		}
	} else {
		c = CHAR_BIT * sizeof(value);
	}
	return c;
}

uint8_t clz_16_ref(uint16_t value) {
	uint8_t c = 0;
	if(value) {
		uint16_t mask = 1U << ((CHAR_BIT * sizeof(value)) - 1);
		while(!(value & mask)) {
			mask >>= 1;
			c++;
		}
	} else {
		c = CHAR_BIT * sizeof(value);
	}
	return c;
}

uint8_t clz_32_ref(uint32_t value) {
	uint8_t c = 0;
	if(value) {
		uint32_t mask = 1UL << ((CHAR_BIT * sizeof(value)) - 1);
		while(!(value & mask)) {
			mask >>= 1;
			c++;
		}
	} else {
		c = CHAR_BIT * sizeof(value);
	}
	return c;
}

uint8_t ffs_8_ref(uint8_t value) {
	return (value ? ctz_8_ref(value) + 1 : 0);
}

uint8_t ffs_16_ref(uint16_t value) {
	return (value ? ctz_16_ref(value) + 1 : 0);
}

uint8_t ffs_32_ref(uint32_t value) {
	return (value ? ctz_32_ref(value) + 1 : 0);
}

uint8_t pop_count_8_ref(uint8_t value) {
	uint8_t c = 0;
	while(value) {
		// Clear the least significant bit set.
		value &= value - 1;
		c++;
	}
	return c;
}

uint8_t pop_count_16_ref(uint16_t value) {
	uint8_t c = 0;
	while(value) {
		// Clear the least significant bit set.
		value &= value - 1;
		c++;
	}
	return c;
}

uint8_t pop_count_32_ref(uint32_t value) {
	uint8_t c = 0;
	while(value) {
		// Clear the least significant bit set.
		value &= value - 1;
		c++;
	}
	return c;
}

uint8_t rotate_left_8_ref(uint8_t value, uint8_t count) {
	const uint8_t mask = CHAR_BIT * sizeof(value) - 1;
	count &= mask;
	return (value << count) | (value >> (-count & mask));
}

uint8_t rotate_right_8_ref(uint8_t value, uint8_t count) {
	const uint8_t mask = CHAR_BIT * sizeof(value) - 1;
	count &= mask;
	return (value >> count) | (value << (-count & mask));
}

uint16_t rotate_left_16_ref(uint16_t value, uint8_t count) {
	const uint8_t mask = CHAR_BIT * sizeof(value) - 1;
	count &= mask;
	return (value << count) | (value >> (-count & mask));
}

uint16_t rotate_right_16_ref(uint16_t value, uint8_t count) {
	const uint8_t mask = CHAR_BIT * sizeof(value) - 1;
	count &= mask;
	return (value >> count) | (value << (-count & mask));
}

uint32_t rotate_left_32_ref(uint32_t value, uint8_t count) {
	const uint8_t mask = CHAR_BIT * sizeof(value) - 1;
	count &= mask;
	return (value << count) | (value >> (-count & mask));
}

uint32_t rotate_right_32_ref(uint32_t value, uint8_t count) {
	const uint8_t mask = CHAR_BIT * sizeof(value) - 1;
	count &= mask;
	return (value >> count) | (value << (-count & mask));
}

void div_s16_ref(int16_t x, int16_t y, div_s16_t *result) {
	result->quot = x / y;
	result->rem = x % y;
}

void div_u16_ref(uint16_t x, uint16_t y, div_u16_t *result) {
	result->quot = x / y;
	result->rem = x % y;
}

void div_u32_ref(uint32_t x, uint32_t y, div_u32_t *result) {
	result->quot = x / y;
	result->rem = x % y;
}
