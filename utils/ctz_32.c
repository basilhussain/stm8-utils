/*******************************************************************************
 *
 * ctz_32.c - 32-bit count trailing zeroes implementation
 *
 * Copyright (c) 2020 Basil Hussain
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

uint8_t ctz_32(uint32_t value) __naked {
	(void)value;

#if defined(CTZ_LUT_LARGE)

	__asm
		; Initialise X reg to zero. This ensures the MSB of table index is zero,
		; but also initialises count to zero, because the LSB is swapped in to
		; the count.
		clrw x

		; Load fourth byte of value arg into A reg. Shift off least-significant
		; bit into carry. Quit if the carry is 1. Look up the zero count for now
		; 7-bit value from the table at offset determined by value byte in X
		; reg, and add it to count.
		ld a, (ASM_ARGS_SP_OFFSET+3, sp)
		srl a
		exg a, xl
		jrc 0001$
		add a, (_ctz_lut_large, x)

		; If the count is now anything less than 8 (i.e. last byte was not all
		; zeroes), then quit.
		cp a, #8
		jrne 0001$

		; Swap the count from A into LSB of X reg for safe-keeping.
		exg a, xl

		; Do the same table look-up as before, but with 3rd byte of value arg.
		ld a, (ASM_ARGS_SP_OFFSET+2, sp)
		srl a
		exg a, xl
		jrc 0001$
		add a, (_ctz_lut_large, x)

		; Quit if count is anything less than 16.
		cp a, #16
		jrne 0001$

		exg a, xl

		; Second byte of value arg.
		ld a, (ASM_ARGS_SP_OFFSET+1, sp)
		srl a
		exg a, xl
		jrc 0001$
		add a, (_ctz_lut_large, x)

		; Quit if count is anything less than 24.
		cp a, #24
		jrne 0001$

		exg a, xl

		; First byte of value arg.
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

	__asm
		; Load value arg into X & Y regs and initialise count in A reg to zero.
		ldw y, (ASM_ARGS_SP_OFFSET+0, sp)
		ldw x, (ASM_ARGS_SP_OFFSET+2, sp)
		clr a

		; Set the carry flag, so that even if value arg is all zeroes, we will
		; always encounter a one after counting all 32 zero bits.
		scf

	0001$:
		; Shift the value right one bit. If the carry (i.e. bit shifted off) is
		; a one, then exit, otherwise increment the count and loop around.
		rrcw y
		rrcw x
		jrc 0002$
		inc a
		jra 0001$

	0002$:
		; Return with the count in the A reg.
		ASM_RETURN
	__endasm;

#endif
}
