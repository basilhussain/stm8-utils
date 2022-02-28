/*******************************************************************************
 *
 * rot_left_8.c - 8-bit left-rotate implementation
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

uint8_t rotate_left_8(uint8_t value, uint8_t count) __naked __stack_args {
	(void)value;
	(void)count;

	// For return value/arg: 0xAA
	// a = 0xAA

	__asm
		; Load count arg into X reg, first masking it to ensure it wraps round
		; if >= 8.
		ld a, (ASM_ARGS_SP_OFFSET+1, sp)
		and a, #0x07
		clrw x
		ld xl, a

		; Load value arg into A register.
		ld a, (ASM_ARGS_SP_OFFSET+0, sp)

		; If the count is zero, quit straight away.
		tnzw x
		jreq 0003$

	0001$:
		; Shift all bits left one place.
		sll a

		; Set least-significant bit of A if bit previously shifted off was 1
		; (i.e. carry flag was set). Use INC instruction as that instruction is
		; smaller (1 byte) versus "add a, #0x01" (2 bytes).
		; TODO: maybe replace both lines with "adc a, #0x00" (so no jump)?
		jrnc 0002$
		inc a

	0002$:
		; Decrement count and loop around if it is not zero.
		decw x
		jrne 0001$

	0003$:
		; The A reg now contains rotated value, so leave it there as function
		; return value.
		ASM_RETURN
	__endasm;
}
