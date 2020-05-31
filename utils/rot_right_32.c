/*******************************************************************************
 *
 * rot_right_32.c - 32-bit right-rotate implementation
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

uint32_t rotate_right_32(uint32_t value, uint8_t count) __naked {
	(void)value;
	(void)count;

	// For return value/arg: 0xAABBCCDD
	// y = 0xAABB (yh = 0xAA, yl = 0xBB)
	// x = 0xCCDD (xh = 0xCC, xl = 0xDD)

	__asm
		; Load value arg into X & Y registers.
		ldw y, (ASM_ARGS_SP_OFFSET+0, sp)
		ldw x, (ASM_ARGS_SP_OFFSET+2, sp)

		; Load count arg into A reg, and mask it to ensure it wraps round if
		; >= 32. If then count is zero, quit straight away.
		ld a, (ASM_ARGS_SP_OFFSET+4, sp)
		and a, #0x1F
		jreq 0003$

	0001$:
		; Shift all bits right one place.
		srlw y
		rrcw x

		; Set most-significant bit of Y if bit previously shifted off was 1
		; (i.e. carry flag was set).
		jrnc 0002$
		addw y, #0x8000

	0002$:
		; Decrement count and loop around if it is not zero.
		dec a
		jrne 0001$

	0003$:
		; The X and Y registers now contain rotated value, so leave them there
		; as function return value.
		ASM_RETURN
	__endasm;
}
