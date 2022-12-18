/*******************************************************************************
 *
 * rot_right_8.c - 8-bit right-rotate implementation
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

uint8_t rotate_right_8(uint8_t value, uint8_t count) __naked __stack_args {
	(void)value;
	(void)count;

	// For return value/arg: 0xAA
	// a = 0xAA

	__asm
		; First mask count arg to ensure it wraps round if >= 8.
		ld a, (ASM_ARGS_SP_OFFSET+1, sp)
		and a, #0x07
		ld (ASM_ARGS_SP_OFFSET+1, sp), a

		; Load value arg into A register.
		ld a, (ASM_ARGS_SP_OFFSET+0, sp)

		; If the count is zero, quit straight away.
		tnz (ASM_ARGS_SP_OFFSET+1, sp)
		jreq 0003$

	0001$:
		; Shift all bits right one place.
		srl a

		; Set most-significant bit of A if bit previously shifted off was 1
		; (i.e. carry flag was set).
		jrnc 0002$
		add a, #0x80

	0002$:
		; Decrement count and loop around if it is not zero.
		dec (ASM_ARGS_SP_OFFSET+1, sp)
		jrne 0001$

	0003$:
		; The A reg now contains rotated value, so leave it there as function
		; return value.
		ASM_RETURN
	__endasm;
}
