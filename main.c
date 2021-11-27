/*******************************************************************************
 *
 * main.c - Test and benchmarking code for STM8 pseudo-intrinsic bit-
 *          manipulation utility function library
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

#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "uart.h"
#include "utils.h"
#include "ref.h"
#include "ucsim.h"

#define CLK_CKDIVR (*(volatile uint8_t *)(0x50C6))

// PC5 is connected to the built-in LED on the STM8S208 Nucleo-64 board.
// Toggling this pin is used for benchmarking.
#define PC_ODR (*(volatile uint8_t *)(0x500A))
#define PC_ODR_ODR5 5
#define PC_DDR (*(volatile uint8_t *)(0x500C))
#define PC_DDR_DDR5 5
#define PC_CR1 (*(volatile uint8_t *)(0x500D))
#define PC_CR1_C15 5

/******************************************************************************/

typedef struct {
	uint16_t pass_count;
	uint16_t fail_count;
} test_result_t;

// Use ANSI terminal escape codes for highlighting pass/fail text.
static const char pass_str[] = "\x1B[1m\x1B[32mPASS\x1B[0m"; // Bold green
static const char fail_str[] = "\x1B[1m\x1B[31mFAIL\x1B[0m"; // Bold red
static const char hrule_str[] = "----------------------------------------";

#define count_test_result(x, r) \
	do { \
		if(x) { \
			(r)->pass_count++; \
		} else { \
			(r)->fail_count++; \
		} \
	} while(0)


#define benchmark_marker_start() do { PC_ODR |= (1 << PC_ODR_ODR5); } while(0)
#define benchmark_marker_end() do { PC_ODR &= ~(1 << PC_ODR_ODR5); } while(0)
#define benchmark(s, o) \
	do { \
		printf("\x1B[1m\x1B[33mBENCHMARK\x1B[0m: " s "\n"); \
		uint16_t n = 10000; \
		benchmark_marker_start(); \
		while(n--) (o); \
		benchmark_marker_end(); \
	} while(0)

/******************************************************************************/

static void test_swap(test_result_t *result) {
	static const uint8_t vals_8[] = {
		0xAB, 0x00, 0xFF,
	};
	static const uint16_t vals_16[] = {
		0xAABB, 0x0000, 0xFFFF, 0xF00F,
	};
	static const uint32_t vals_32[] = {
		0xAABBCCDDUL, 0x00000000UL, 0xFFFFFFFFUL, 0x5555AAAAUL,
	};

	for(size_t i = 0; i < (sizeof(vals_8) / sizeof(vals_8[0])); i++) {
		uint8_t foo = swap_ref(vals_8[i]);
		uint8_t bar = swap(vals_8[i]);
		printf("0x%02X: swap_ref = 0x%02X, swap = 0x%02X - %s\n", vals_8[i], foo, bar, (foo == bar ? pass_str : fail_str));
		count_test_result(foo == bar, result);
	}

	for(size_t i = 0; i < (sizeof(vals_16) / sizeof(vals_16[0])); i++) {
		uint16_t foo = bswap_16_ref(vals_16[i]);
		uint16_t bar = bswap_16(vals_16[i]);
		printf("0x%04X: bswap_16_ref = 0x%04X, bswap_16 = 0x%04X - %s\n", vals_16[i], foo, bar, (foo == bar ? pass_str : fail_str));
		count_test_result(foo == bar, result);
	}

	for(size_t i = 0; i < (sizeof(vals_32) / sizeof(vals_32[0])); i++) {
		uint32_t foo = bswap_32_ref(vals_32[i]);
		uint32_t bar = bswap_32(vals_32[i]);
		printf("0x%08lX: bswap_32_ref = 0x%08lX, bswap_32 = 0x%08lX - %s\n", vals_32[i], foo, bar, (foo == bar ? pass_str : fail_str));
		count_test_result(foo == bar, result);
	}
}

static void benchmark_swap(void) {
	static const uint8_t val_8 = 0xAB;
	static const uint16_t val_16 = 0xAABB;
	static const uint32_t val_32 = 0xAABBCCDDUL;

	benchmark("swap_ref", swap_ref(val_8));
	benchmark("swap", swap(val_8));

	benchmark("bswap_16_ref", bswap_16_ref(val_16));
	benchmark("bswap_16", bswap_16(val_16));

	benchmark("bswap_32_ref", bswap_32_ref(val_32));
	benchmark("bswap_32", bswap_32(val_32));
}

static void test_pop_count(test_result_t *result) {
	static const uint8_t vals_8[] = {
		0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F,
		0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF,
		0xAA, 0x55, 0x77,
	};
	static const uint16_t vals_16[] = {
		0x0000, 0x0001, 0x0003, 0x0007, 0x000F, 0x001F, 0x003F, 0x007F, 0x00FF,
		0x01FF, 0x03FF, 0x07FF, 0x0FFF, 0x1FFF, 0x3FFF, 0x7FFF, 0x8000, 0xC000,
		0xE000, 0xF000, 0xF800, 0xFC00, 0xFE00, 0xFF00, 0xFF80, 0xFFC0, 0xFFE0,
		0xFFF0, 0xFFF8, 0xFFFC, 0xFFFE, 0xFFFF, 0xAAAA, 0x5555, 0x1234,
	};
	static const uint32_t vals_32[] = {
		0x00000000UL, 0x00000001UL, 0x00000003UL, 0x00000007UL, 0x0000000FUL,
		0x0000001FUL, 0x0000003FUL, 0x0000007FUL, 0x000000FFUL, 0x000001FFUL,
		0x000003FFUL, 0x000007FFUL, 0x00000FFFUL, 0x00001FFFUL, 0x00003FFFUL,
		0x00007FFFUL, 0x0000FFFFUL, 0x0001FFFFUL, 0x0003FFFFUL, 0x0007FFFFUL,
		0x000FFFFFUL, 0x001FFFFFUL, 0x003FFFFFUL, 0x007FFFFFUL, 0x00FFFFFFUL,
		0x01FFFFFFUL, 0x03FFFFFFUL, 0x07FFFFFFUL, 0x0FFFFFFFUL, 0x1FFFFFFFUL,
		0x3FFFFFFFUL, 0x7FFFFFFFUL, 0xFFFFFFFFUL,
	};

	for(size_t i = 0; i < (sizeof(vals_8) / sizeof(vals_8[0])); i++) {
		uint8_t foo = pop_count_8_ref(vals_8[i]);
		uint8_t bar = pop_count_8(vals_8[i]);
		printf("0x%02X: pop_count_8_ref = %u, pop_count_8 = %u - %s\n", vals_8[i], foo, bar, (foo == bar ? pass_str : fail_str));
		count_test_result(foo == bar, result);
	}

	for(size_t i = 0; i < (sizeof(vals_16) / sizeof(vals_16[0])); i++) {
		uint8_t foo = pop_count_16_ref(vals_16[i]);
		uint8_t bar = pop_count_16(vals_16[i]);
		printf("0x%04X: pop_count_16_ref = %u, pop_count_16 = %u - %s\n", vals_16[i], foo, bar, (foo == bar ? pass_str : fail_str));
		count_test_result(foo == bar, result);
	}

	for(size_t i = 0; i < (sizeof(vals_32) / sizeof(vals_32[0])); i++) {
		uint8_t foo = pop_count_32_ref(vals_32[i]);
		uint8_t bar = pop_count_32(vals_32[i]);
		printf("0x%08lX: pop_count_32_ref = %u, pop_count_32 = %u - %s\n", vals_32[i], foo, bar, (foo == bar ? pass_str : fail_str));
		count_test_result(foo == bar, result);
	}
}

static void benchmark_pop_count(void) {
	static const uint8_t val_8 = 0x55;
	static const uint16_t val_16 = 0x5555;
	static const uint32_t val_32 = 0x55555555UL;

	benchmark("pop_count_8_ref", pop_count_8_ref(val_8));
	benchmark("pop_count_8", pop_count_8(val_8));

	benchmark("pop_count_16_ref", pop_count_16_ref(val_16));
	benchmark("pop_count_16", pop_count_16(val_16));

	benchmark("pop_count_32_ref", pop_count_32_ref(val_32));
	benchmark("pop_count_32", pop_count_32(val_32));
}

static void test_ctz_clz_ffs(test_result_t *result) {
	static const uint8_t vals_8[] = {
		0x00, 0x01, 0x10, 0xFF, 0x0E, 0x0C, 0x08, 0xE0, 0xC0, 0x80, 0x5A, 0x88,
	};
	static const uint16_t vals_16[] = {
		0x0000, 0x0001, 0x0010, 0x0100, 0x1000, 0xFFFF, 0xFFF0, 0xFF00, 0xF000,
		0x000E, 0x000C, 0x0008, 0x00E0, 0x00C0, 0x0080, 0x0E00, 0x0C00, 0x0800,
		0xE000, 0xC000, 0x8000, 0x1110, 0x8888,
	};
	static const uint32_t vals_32[] = {
		0x00000000UL, 0x00000001UL, 0x00000010UL, 0x00000100UL, 0x00001000UL,
		0x00010000UL, 0x00100000UL, 0x01000000UL, 0x10000000UL, 0xFFFFFFFFUL,
		0x80808080UL, 0x00555500UL, 0xAA0AA000UL,
	};

	for(size_t i = 0; i < (sizeof(vals_8) / sizeof(vals_8[0])); i++) {
		uint8_t foo, bar;
		printf("0x%02X:\n", vals_8[i]);
		foo = ctz_8_ref(vals_8[i]);
		bar = ctz_8(vals_8[i]);
		printf("  ctz_8_ref = %u, ctz_8 = %u - %s\n", foo, bar, (foo == bar ? pass_str : fail_str));
		count_test_result(foo == bar, result);
		foo = clz_8_ref(vals_8[i]);
		bar = clz_8(vals_8[i]);
		printf("  clz_8_ref = %u, clz_8 = %u - %s\n", foo, bar, (foo == bar ? pass_str : fail_str));
		count_test_result(foo == bar, result);
		foo = ffs_8_ref(vals_8[i]);
		bar = ffs_8(vals_8[i]);
		printf("  ffs_8_ref = %u, ffs_8 = %u - %s\n", foo, bar, (foo == bar ? pass_str : fail_str));
		count_test_result(foo == bar, result);
	}

	for(size_t i = 0; i < (sizeof(vals_16) / sizeof(vals_16[0])); i++) {
		uint8_t foo, bar;
		printf("0x%04X:\n", vals_16[i]);
		foo = ctz_16_ref(vals_16[i]);
		bar = ctz_16(vals_16[i]);
		printf("  ctz_16_ref = %u, ctz_16 = %u - %s\n", foo, bar, (foo == bar ? pass_str : fail_str));
		count_test_result(foo == bar, result);
		foo = clz_16_ref(vals_16[i]);
		bar = clz_16(vals_16[i]);
		printf("  clz_16_ref = %u, clz_16 = %u - %s\n", foo, bar, (foo == bar ? pass_str : fail_str));
		count_test_result(foo == bar, result);
		foo = ffs_16_ref(vals_16[i]);
		bar = ffs_16(vals_16[i]);
		printf("  ffs_16_ref = %u, ffs_16 = %u - %s\n", foo, bar, (foo == bar ? pass_str : fail_str));
		count_test_result(foo == bar, result);
	}

	for(size_t i = 0; i < (sizeof(vals_32) / sizeof(vals_32[0])); i++) {
		uint8_t foo, bar;
		printf("0x%08lX:\n", vals_32[i]);
		foo = ctz_32_ref(vals_32[i]);
		bar = ctz_32(vals_32[i]);
		printf("  ctz_32_ref = %u, ctz_32 = %u - %s\n", foo, bar, (foo == bar ? pass_str : fail_str));
		count_test_result(foo == bar, result);
		foo = clz_32_ref(vals_32[i]);
		bar = clz_32(vals_32[i]);
		printf("  clz_32_ref = %u, clz_32 = %u - %s\n", foo, bar, (foo == bar ? pass_str : fail_str));
		count_test_result(foo == bar, result);
		foo = ffs_32_ref(vals_32[i]);
		bar = ffs_32(vals_32[i]);
		printf("  ffs_32_ref = %u, ffs_32 = %u - %s\n", foo, bar, (foo == bar ? pass_str : fail_str));
		count_test_result(foo == bar, result);
	}
}

static void benchmark_ctz_clz_ffs(void) {
	static const uint8_t val_8 = 0x18;
	static const uint16_t val_16 = 0x0180;
	static const uint32_t val_32 = 0x00018000UL;

	benchmark("ctz_8_ref", ctz_8_ref(val_8));
	benchmark("ctz_8", ctz_8(val_8));

	benchmark("ctz_16_ref", ctz_16_ref(val_16));
	benchmark("ctz_16", ctz_16(val_16));

	benchmark("ctz_32_ref", ctz_32_ref(val_32));
	benchmark("ctz_32", ctz_32(val_32));

	benchmark("clz_8_ref", clz_8_ref(val_8));
	benchmark("clz_8", clz_8(val_8));

	benchmark("clz_16_ref", clz_16_ref(val_16));
	benchmark("clz_16", clz_16(val_16));

	benchmark("clz_32_ref", clz_32_ref(val_32));
	benchmark("clz_32", clz_32(val_32));

	benchmark("ffs_8_ref", ffs_8_ref(val_8));
	benchmark("ffs_8", ffs_8(val_8));

	benchmark("ffs_16_ref", ffs_16_ref(val_16));
	benchmark("ffs_16", ffs_16(val_16));

	benchmark("ffs_32_ref", ffs_32_ref(val_32));
	benchmark("ffs_32", ffs_32(val_32));
}

static void test_rotate(test_result_t *result) {
	static const uint8_t vals_8[] = {
		0x80, 0x01, 0xFF, 0x00, 0xDD,
	};
	static const uint16_t vals_16[] = {
		0x8000, 0x0001, 0xFFFF, 0x0000, 0xAABB,
	};
	static const uint32_t vals_32[] = {
		0x80000000UL, 0x00000001UL, 0xFFFFFFFFUL, 0x00000000UL, 0xAABBCCDDUL,
	};

	for(size_t i = 0; i < (sizeof(vals_8) / sizeof(vals_8[0])); i++) {
		printf("value = 0x%02X\n", vals_8[i]);
		for(uint8_t j = 0; j < 11; j++) {
			uint8_t foo = rotate_left_8_ref(vals_8[i], j);
			uint8_t bar = rotate_left_8(vals_8[i], j);
			printf("  %2u: rotate_left_8_ref = 0x%02X, rotate_left_8 = 0x%02X - %s\n", j, foo, bar, (foo == bar ? pass_str : fail_str));
			count_test_result(foo == bar, result);
		}
	}

	for(size_t i = 0; i < (sizeof(vals_8) / sizeof(vals_8[0])); i++) {
		printf("value = 0x%02X\n", vals_8[i]);
		for(uint8_t j = 0; j < 11; j++) {
			uint8_t foo = rotate_right_8_ref(vals_8[i], j);
			uint8_t bar = rotate_right_8(vals_8[i], j);
			printf("  %2u: rotate_right_8_ref = 0x%02X, rotate_right_8 = 0x%02X - %s\n", j, foo, bar, (foo == bar ? pass_str : fail_str));
			count_test_result(foo == bar, result);
		}
	}

	for(size_t i = 0; i < (sizeof(vals_16) / sizeof(vals_16[0])); i++) {
		printf("value = 0x%04X\n", vals_16[i]);
		for(uint8_t j = 0; j < 19; j++) {
			uint16_t foo = rotate_left_16_ref(vals_16[i], j);
			uint16_t bar = rotate_left_16(vals_16[i], j);
			printf("  %2u: rotate_left_16_ref = 0x%04X, rotate_left_16 = 0x%04X - %s\n", j, foo, bar, (foo == bar ? pass_str : fail_str));
			count_test_result(foo == bar, result);
		}
	}

	for(size_t i = 0; i < (sizeof(vals_16) / sizeof(vals_16[0])); i++) {
		printf("value = 0x%04X\n", vals_16[i]);
		for(uint8_t j = 0; j < 19; j++) {
			uint16_t foo = rotate_right_16_ref(vals_16[i], j);
			uint16_t bar = rotate_right_16(vals_16[i], j);
			printf("  %2u: rotate_right_16_ref = 0x%04X, rotate_right_16 = 0x%04X - %s\n", j, foo, bar, (foo == bar ? pass_str : fail_str));
			count_test_result(foo == bar, result);
		}
	}

	for(size_t i = 0; i < (sizeof(vals_32) / sizeof(vals_32[0])); i++) {
		printf("value = 0x%08lX\n", vals_32[i]);
		for(uint8_t j = 0; j < 35; j++) {
			uint32_t foo = rotate_left_32_ref(vals_32[i], j);
			uint32_t bar = rotate_left_32(vals_32[i], j);
			printf("  %2u: rotate_left_32_ref = 0x%08lX, rotate_left_32 = 0x%08lX - %s\n", j, foo, bar, (foo == bar ? pass_str : fail_str));
			count_test_result(foo == bar, result);
		}
	}

	for(size_t i = 0; i < (sizeof(vals_32) / sizeof(vals_32[0])); i++) {
		printf("value = 0x%08lX\n", vals_32[i]);
		for(uint8_t j = 0; j < 35; j++) {
			uint32_t foo = rotate_right_32_ref(vals_32[i], j);
			uint32_t bar = rotate_right_32(vals_32[i], j);
			printf("  %2u: rotate_right_32_ref = 0x%08lX, rotate_right_32 = 0x%08lX - %s\n", j, foo, bar, (foo == bar ? pass_str : fail_str));
			count_test_result(foo == bar, result);
		}
	}
}

static void benchmark_rotate(void) {
	static const uint8_t val_8 = 0x55;
	static const uint16_t val_16 = 0x5555;
	static const uint32_t val_32 = 0x55555555UL;

#define rotate_count(v) (((CHAR_BIT * sizeof(v)) / 4) * 3)

	benchmark("rotate_left_8_ref", rotate_left_8_ref(val_8, rotate_count(val_8)));
	benchmark("rotate_left_8", rotate_left_8(val_8, rotate_count(val_8)));

	benchmark("rotate_left_16_ref", rotate_left_16_ref(val_16, rotate_count(val_16)));
	benchmark("rotate_left_16", rotate_left_16(val_16, rotate_count(val_16)));

	benchmark("rotate_left_32_ref", rotate_left_32_ref(val_32, rotate_count(val_32)));
	benchmark("rotate_left_32", rotate_left_32(val_32, rotate_count(val_32)));

	benchmark("rotate_right_8_ref", rotate_right_8_ref(val_8, rotate_count(val_8)));
	benchmark("rotate_right_8", rotate_right_8(val_8, rotate_count(val_8)));

	benchmark("rotate_right_16_ref", rotate_right_16_ref(val_16, rotate_count(val_16)));
	benchmark("rotate_right_16", rotate_right_16(val_16, rotate_count(val_16)));

	benchmark("rotate_right_32_ref", rotate_right_32_ref(val_32, rotate_count(val_32)));
	benchmark("rotate_right_32", rotate_right_32(val_32, rotate_count(val_32)));

#undef rotate_count
}

/*
static void benchmark_rotate_linearity(void) {
	static const uint32_t val_32 = 0x55555555UL;

	for(uint8_t i = 0; i < 35; i++) {
		printf("%u\n", i);

		benchmark_marker_start();
		rotate_left_32_ref(val_32, i);
		benchmark_marker_end();

		benchmark_marker_start();
		rotate_left_32(val_32, i);
		benchmark_marker_end();
	}
}
*/

static void test_div(test_result_t *result) {
	static const struct {
		int16_t a;
		int16_t b;
	} vals_s[] = {
		{ 0, 1 },
		{ 1, 1 },
		{ 1, 2 },
		{ -1, 1 },
		{ -1, 2 },
		{ 1, -1 },
		{ 1, -2 },
		{ -1, -1 },
		{ -1, -2 },
		{ 1000, 2 },
		{ -1000, 2 },
		{ 10000, 300 },
		{ 10000, -300 },
		{ 32767, 3 },
		{ -32768, 3 },
	};
	static const struct {
		uint16_t a;
		uint16_t b;
	} vals_u[] = {
		{ 0, 1 },
		{ 1, 1 },
		{ 1, 2 },
		{ 1000, 2 },
		{ 10000, 300 },
		{ 32767, 3 },
		{ 65535, 3 },
		{ 65535, 10000 },
		{ 65535, 65535 },
	};
	div_s16_t foo_s, bar_s;
	div_u16_t foo_u, bar_u;
	bool pass_fail;

	for(size_t i = 0; i < (sizeof(vals_s) / sizeof(vals_s[0])); i++) {
		div_s16_ref(vals_s[i].a, vals_s[i].b, &foo_s);
		div_s16(vals_s[i].a, vals_s[i].b, &bar_s);
		pass_fail = (foo_s.quot == bar_s.quot && foo_s.rem == bar_s.rem && ((bar_s.quot * vals_s[i].b) + bar_s.rem) == vals_s[i].a);
		printf(
			"%d, %d: div_s16_ref = { quot = %d, rem = %d }, div_s16 = { quot = %d, rem = %d } - %s\n",
			vals_s[i].a,
			vals_s[i].b,
			foo_s.quot,
			foo_s.rem,
			bar_s.quot,
			bar_s.rem,
			(pass_fail ? pass_str : fail_str)
		);
		count_test_result(pass_fail, result);
	}

	for(size_t i = 0; i < (sizeof(vals_u) / sizeof(vals_u[0])); i++) {
		div_u16_ref(vals_u[i].a, vals_u[i].b, &foo_u);
		div_u16(vals_u[i].a, vals_u[i].b, &bar_u);
		pass_fail = (foo_u.quot == bar_u.quot && foo_u.rem == bar_u.rem && ((bar_u.quot * vals_u[i].b) + bar_u.rem) == vals_u[i].a);
		printf(
			"%u, %u: div_u16_ref = { quot = %u, rem = %u }, div_u16 = { quot = %u, rem = %u } - %s\n",
			vals_u[i].a,
			vals_u[i].b,
			foo_u.quot,
			foo_u.rem,
			bar_u.quot,
			bar_u.rem,
			(pass_fail ? pass_str : fail_str)
		);
		count_test_result(pass_fail, result);
	}
}

static void benchmark_div(void) {
	static const int16_t val_s_a = -3000;
	static const int16_t val_s_b = 45;
	static const uint16_t val_u_a = 47832;
	static const uint16_t val_u_b = 900;
	div_s16_t foo;
	div_u16_t bar;

	benchmark("div_s16_ref", div_s16_ref(val_s_a, val_s_b, &foo));
	benchmark("div_s16", div_s16(val_s_a, val_s_b, &foo));
	benchmark("div_u16_ref", div_u16_ref(val_u_a, val_u_b, &bar));
	benchmark("div_u16", div_u16(val_u_a, val_u_b, &bar));
}

void main(void) {
	test_result_t results = { 0, 0 };

	CLK_CKDIVR = 0;
	PC_DDR = (1 << PC_DDR_DDR5);
	PC_CR1 = (1 << PC_CR1_C15);

	if(ucsim_if_detect()) {
		uart_init(UART_BAUD_115200, ucsim_if_putchar, NULL);
	} else {
		uart_init(UART_BAUD_115200, uart_putchar, uart_getchar);
	}

	puts(hrule_str);

	test_swap(&results);
	test_ctz_clz_ffs(&results);
	test_pop_count(&results);
	test_rotate(&results);
	test_div(&results);

	printf("TOTAL RESULTS: passed = %u, failed = %u\n", results.pass_count, results.fail_count);

	puts(hrule_str);

	benchmark_swap();
	benchmark_pop_count();
	benchmark_ctz_clz_ffs();
	benchmark_rotate();
	benchmark_div();

	puts(hrule_str);

	/*
	benchmark_rotate_linearity();

	puts(hrule_str);
	*/

	while(1);
}
