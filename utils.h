/*******************************************************************************
 *
 * utils.h - Header for STM8 pseudo-intrinsic bit-manipulation utility functions
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

#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>

// Short-term fix to force usage of old ABI when compiled with SDCC v4.2.0
// (or newer). New ABI passes simple arguments (e.g. one or two 8- or 16-bit
// values) in A/X registers, versus previous where all args are on the stack.
// Eventually, all assembly code should be changed to use new ABI. May actually
// offer a performance increase, eliminating code that simply takes arg values
// and loads them to A/X registers.
#if defined(__SDCCCALL) && __SDCCCALL != 0
#define __stack_args __sdcccall(0)
#else
#define __stack_args
#endif

typedef struct {
	int16_t quot;
	int16_t rem;
} div_s16_t;

typedef struct {
	uint16_t quot;
	uint16_t rem;
} div_u16_t;

typedef div_s16_t div_t;

/******************************************************************************/

extern uint8_t swap(const uint8_t value) __naked __stack_args;
extern uint16_t bswap_16(const uint16_t value) __naked __stack_args;
extern uint32_t bswap_32(const uint32_t value) __naked __stack_args;

extern uint8_t pop_count_8(uint8_t value) __naked __stack_args;
extern uint8_t pop_count_16(uint16_t value) __naked __stack_args;
extern uint8_t pop_count_32(uint32_t value) __naked __stack_args;

extern uint8_t ctz_8(uint8_t value) __naked __stack_args;
extern uint8_t ctz_16(uint16_t value) __naked __stack_args;
extern uint8_t ctz_32(uint32_t value) __naked __stack_args;

extern uint8_t clz_8(uint8_t value) __naked __stack_args;
extern uint8_t clz_16(uint16_t value) __naked __stack_args;
extern uint8_t clz_32(uint32_t value) __naked __stack_args;

extern uint8_t ffs_8(uint8_t value) __stack_args;
extern uint8_t ffs_16(uint16_t value) __stack_args;
extern uint8_t ffs_32(uint32_t value) __stack_args;

extern uint8_t rotate_left_8(uint8_t value, uint8_t count) __naked __stack_args;
extern uint8_t rotate_right_8(uint8_t value, uint8_t count) __naked __stack_args;
extern uint16_t rotate_left_16(uint16_t value, uint8_t count) __naked __stack_args;
extern uint16_t rotate_right_16(uint16_t value, uint8_t count) __naked __stack_args;
extern uint32_t rotate_left_32(uint32_t value, uint8_t count) __naked __stack_args;
extern uint32_t rotate_right_32(uint32_t value, uint8_t count) __naked __stack_args;

extern void div_s16(int16_t x, int16_t y, div_s16_t *result) __naked __stack_args;
extern void div_u16(uint16_t x, uint16_t y, div_u16_t *result) __naked __stack_args;

extern int strctcmp(const char *s1, const char *s2) __naked __stack_args;

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
