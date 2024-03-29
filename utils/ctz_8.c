/*******************************************************************************
 *
 * ctz_8.c - 8-bit count trailing zeroes implementation
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

#include <stdint.h>
#include "common.h"
#include "../utils.h"

uint8_t ctz_8(uint8_t value) __naked __stack_args {
	(void)value;

#if defined(CTZ_LUT_LARGE)

	__asm
		; Initialise X reg to zero. This ensures the MSB of table index is zero,
		; but also initialises count to zero, because the LSB is swapped in to
		; the count.
		clrw x

		; Load value arg into A reg. Shift off least-significant bit into carry.
		; Quit if the carry is 1. Look up the zero count for now 7-bit value
		; from the table at offset determined by value byte in X reg, and add it
		; to count.
		ld a, (ASM_ARGS_SP_OFFSET+0, sp)
		srl a
		exg a, xl
		jrc 0001$
		add a, (_ctz_lut_large, x)

	0001$:
		; Return with the count in the A reg.
		ASM_RETURN
	__endasm;

#else

	// 10+5N cycles (where N = num trailing zeroes)
	__asm
		; Load value arg into A reg and initialise count in X reg to zero.
		ld a, (ASM_ARGS_SP_OFFSET+0, sp)
		clrw x

		; Set the carry flag, so that even if value arg is all zeroes, we will
		; always encounter a one after counting all 8 zero bits.
		scf

	0001$:
		; Shift the value right one bit. If the carry (i.e. bit shifted off) is
		; a one, then exit, otherwise increment the count and loop around.
		rrc a
		jrc 0002$
		incw x
		jra 0001$

	0002$:
		; Copy the count to the A reg for return value.
		ld a, xl
		ASM_RETURN
	__endasm;

#endif
}
