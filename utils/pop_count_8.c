/*******************************************************************************
 *
 * pop_count_8.c - 8-bit population count implementation
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

uint8_t pop_count_8(uint8_t value) __naked {
	(void)value;

#if defined(POP_COUNT_LUT_LARGE)

	// 9-10 cycles, 128 bytes LUT - trades a couple of extra cycles for table
	// that is half the size compared to 256 byte LUT.
	__asm
		; Initialise X reg to zero. This ensures the MSB of table index is zero,
		; but also initialises count to zero, because the LSB is swapped in to
		; the count.
		clrw x

		; Load value arg into A reg. Shift off least-significant bit into carry,
		; then swap in to LSB of X reg. Look up the bit count for now 7-bit
		; value from the table at offset determined by value byte in X reg, and
		; add it together with carry bit to count.
		ld a, (ASM_ARGS_SP_OFFSET+0, sp)
		srl a
		exg a, xl
		adc a, (_pop_count_lut_large, x)

		; Return with the count in the A reg.
		ASM_RETURN
	__endasm;

#elif defined(POP_COUNT_LUT_SMALL)

	// 26-27 cycles, 16 bytes LUT
	__asm
		; Initialise X reg to zero. This ensures the MSB of table index is zero,
		; but also initialises count to zero, because the LSB is swapped in to
		; the count.
		clrw x

		; Load value arg into A reg, mask out the high nibble (retaining low
		; nibble), then swap in to LSB of X reg. Look up the bit count from the
		; table at offset determined by value nibble in X reg.
		ld a, (ASM_ARGS_SP_OFFSET+0, sp)
		and a, #0x0F
		exg a, xl
		add a, (_pop_count_lut_small, x)

		; Swap the count from A into X reg for safe-keeping.
		exg a, xl

		; Re-load value arg into A, but swap nibbles so we work with high
		; nibble. Mask and look-up and add to bit count as before.
		ld a, (ASM_ARGS_SP_OFFSET+0, sp)
		swap a
		and a, #0x0F
		exg a, xl
		add a, (_pop_count_lut_small, x)

		; Return with the count in the A reg.
		ASM_RETURN
	__endasm;

#else

	__asm
		; Initialise X reg to zero. Load value arg into A register, then swap it
		; into LSB of X reg. This also initialises A, the count, to zero.
		clrw x
		ld a, (ASM_ARGS_SP_OFFSET+0, sp)
		exg a, xl

	0001$:
		; Shift the value right one bit, then add carry (i.e. bit shifted off)
		; to count. Keep looping and doing this until value is non-zero.
		srlw x
		adc a, #0
		tnzw x
		jrne 0001$

		; The A reg already contains count, so leave it there as function
		; return value.
		ASM_RETURN
	__endasm;

#endif
}
