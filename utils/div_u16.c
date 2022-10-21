/*******************************************************************************
 *
 * div_u16.c - 16-bit simultaneous quotient and remainder unsigned division
 *             implementation
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

// TODO: perhaps make return value of function carry value from DIVW, so
// divide-by-zero can be ascertained.

void div_u16(uint16_t x, uint16_t y, div_u16_t *result) __naked __stack_args {
	(void)x;
	(void)y;
	(void)result;

	__asm
		; Load x argument dividend and y argument divisor.
		ldw x, (ASM_ARGS_SP_OFFSET+0, sp)
		ldw y, (ASM_ARGS_SP_OFFSET+2, sp)

		; Do the division.
		divw x, y

		; Temporarily copy quotient and remainder values to stack args (rather
		; than pushing anew to stack).
		ldw (ASM_ARGS_SP_OFFSET+0, sp), x
		ldw (ASM_ARGS_SP_OFFSET+2, sp), y

		; Copy the quotient and remainder values into output struct via the
		; given pointer.
		ldw x, (ASM_ARGS_SP_OFFSET+4, sp)
		ldw y, (ASM_ARGS_SP_OFFSET+0, sp)
		ldw (x), y
		ldw y, (ASM_ARGS_SP_OFFSET+2, sp)
		ldw (2, x), y

		ASM_RETURN
	__endasm;
}
