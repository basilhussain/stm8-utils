/*******************************************************************************
 *
 * reflect_8.c - 8-bit reversal/reflection implementation
 *
 * Copyright (c) 2023 Basil Hussain
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

uint8_t reflect_8(uint8_t value) __naked __stack_args {
	(void)value;

#ifdef REFLECT_LUT

	// 12 cycles (exc. return), 24+16=40 bytes
	__asm
		; Load and mask upper nibble of argument value, swap it to lower
		; position, and put into X reg.
		ld a, (ASM_ARGS_SP_OFFSET+0, sp)
		and a, #0xF0
		swap a
		clrw x
		ld xl, a

		; Load and mask lower nibble of argument value and put into Y reg.
		ld a, (ASM_ARGS_SP_OFFSET+0, sp)
		and a, #0x0F
		clrw y
		ld yl, a

		; Lookup lower nibble reflected value in table and swap it into upper
		; nibble of result. Then do same for upper nibble and put into lower
		; nibble of result.
		ld a, (_reflect_lut, y)
		swap a
		or a, (_reflect_lut, x)

		; Return value is already in A reg.
		ASM_RETURN
	__endasm;

#else

	// unrolled 16 cycles (exc. return), 25 bytes
	__asm
		.macro reflect_8_shift
			srl (ASM_ARGS_SP_OFFSET+0, sp)
			rlc a
		.endm

#ifdef REFLECT_UNROLLED

		.rept 8
			reflect_8_shift
		.endm

#else

		ldw x, #8

	0001$:
		reflect_8_shift
		decw x
		jrne 0001$

#endif

		; Return value is already in A reg.
		ASM_RETURN
	__endasm;

#endif
}
