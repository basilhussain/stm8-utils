/*******************************************************************************
 *
 * utils.h - Header for STM8 pseudo-intrinsic bit-manipulation utility functions
 *
 * Copyright (c) 2021 Basil Hussain
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

#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>

typedef struct {
	int quot;
	int rem;
} div_s16_t;

typedef div_s16_t div_t;

/******************************************************************************/

extern uint8_t swap(const uint8_t value);
extern uint16_t bswap_16(const uint16_t value);
extern uint32_t bswap_32(const uint32_t value);

extern uint8_t pop_count_8(uint8_t value);
extern uint8_t pop_count_16(uint16_t value);
extern uint8_t pop_count_32(uint32_t value);

extern uint8_t ctz_8(uint8_t value);
extern uint8_t ctz_16(uint16_t value);
extern uint8_t ctz_32(uint32_t value);

extern uint8_t clz_8(uint8_t value);
extern uint8_t clz_16(uint16_t value);
extern uint8_t clz_32(uint32_t value);

extern uint8_t ffs_8(uint8_t value);
extern uint8_t ffs_16(uint16_t value);
extern uint8_t ffs_32(uint32_t value);

extern uint8_t rotate_left_8(uint8_t value, uint8_t count);
extern uint8_t rotate_right_8(uint8_t value, uint8_t count);
extern uint16_t rotate_left_16(uint16_t value, uint8_t count);
extern uint16_t rotate_right_16(uint16_t value, uint8_t count);
extern uint32_t rotate_left_32(uint32_t value, uint8_t count);
extern uint32_t rotate_right_32(uint32_t value, uint8_t count);

extern void div_s16(int16_t x, int16_t y, div_s16_t *result);

/******************************************************************************/

#define parity_even_8(x) (pop_count_8(x) & (uint8_t)0x01)
#define parity_even_16(x) (pop_count_16(x) & (uint8_t)0x01)
#define parity_even_32(x) (pop_count_32(x) & (uint8_t)0x01)

#define parity_odd_8(x) (~pop_count_8(x) & (uint8_t)0x01)
#define parity_odd_16(x) (~pop_count_16(x) & (uint8_t)0x01)
#define parity_odd_32(x) (~pop_count_32(x) & (uint8_t)0x01)

/******************************************************************************/

// Some function aliases to match names used by some standard libraries or
// compiler built-ins.
#define popcount(x) pop_count_16(x)
#define popcountl(x) pop_count_32(x)
#define ctz(x) ctz_16(x)
#define ctzl(x) ctz_32(x)
#define clz(x) clz_16(x)
#define clzl(x) clz_32(x)
#define ffs(x) ffs_16(x)
#define ffsl(x) ffs_32(x)
#define parity(x) parity_even_16(x)
#define parityl(x) parity_even_32(x)
#define div(x, y, r) div_s16(x, y, r)

#endif // UTILS_H_
