/*******************************************************************************
 *
 * rot_left_16.c - 16-bit left-rotate implementation
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

uint16_t rotate_left_16(uint16_t value, uint8_t count) __naked __stack_args {
	(void)value;
	(void)count;

	// For return value/arg: 0xAABB
	// x = 0xAABB (xh = 0xAA, xl = 0xBB)

	__asm
		; Load value arg into X register.
		ldw x, (ASM_ARGS_SP_OFFSET+0, sp)

		; Load count arg into A reg, and mask it to ensure it wraps round if
		; >= 16. If then count is zero, quit straight away.
		ld a, (ASM_ARGS_SP_OFFSET+2, sp)
		and a, #0x0F
		jreq 0003$

	0001$:
		; Shift all bits left one place.
		sllw x

		; Set least-significant bit of X if bit previously shifted off was 1
		; (i.e. carry flag was set). Use INCW instruction as it only takes one
		; cycle versus two for "addw x, #0x0001".
		jrnc 0002$
		incw x

	0002$:
		; Decrement count and loop around if it is not zero.
		dec a
		jrne 0001$

	0003$:
		; The X reg now contains rotated value, so leave it there as function
		; return value.
		ASM_RETURN
	__endasm;
}
