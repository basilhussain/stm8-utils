/*******************************************************************************
 *
 * div.c - Simultaneous quotient and remainder division implementation
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

#include <stdint.h>
#include "common.h"
#include "../utils.h"

// TODO: div_u16 for unsigned int? Won't need any of the negating code.
// TODO: perhaps make return value of function carry value from DIVW, so
// divide-by-zero can be ascertained.

void div_s16(int16_t x, int16_t y, div_s16_t *result) {
	(void)x;
	(void)y;
	(void)result;

	__asm
		; Make room on stack for flag vars denoting if args are negative (with
		; default value of zero).
		push #0 ; x
		push #0 ; y

		; Load x argument dividend and check if negative.
		ldw x, (ASM_ARGS_SP_OFFSET+2+0, sp)
		jrpl 0001$

		; Negate x value to make positive and set flag indicating it was
		; negative.
		negw x
		inc (2, sp)

	0001$:
		; Load y argument divisor and check if negative.
		ldw y, (ASM_ARGS_SP_OFFSET+2+2, sp)
		jrpl 0002$

		; Negate y value to make positive and set flag indicating it was
		; negative.
		negw y
		inc (1, sp)

	0002$:
		; Do the division.
		divw x, y

		; If either of the argument values were originally negative, but not
		; both, then we need to negate the resultant quotient.
		ld a, (2, sp)
		xor a, (1, sp)
		jreq 0003$

		; Negate the quotient.
		negw x

	0003$:
		; Remainder needs to take the sign of dividend (x argument). If that was
		; originally negative, then negate the remainder.
		tnz (2, sp)
		jreq 0004$

		; Negate the remainder.
		negw y

	0004$:
		; Temporarily copy quotient and remainder values to stack args (rather
		; than pushing anew to stack).
		ldw (ASM_ARGS_SP_OFFSET+2+0, sp), x
		ldw (ASM_ARGS_SP_OFFSET+2+2, sp), y

		; Copy the quotient and remainder values into output struct via the
		; given pointer.
		ldw x, (ASM_ARGS_SP_OFFSET+2+4, sp)
		ldw y, (ASM_ARGS_SP_OFFSET+2+0, sp)
		ldw (x), y
		ldw y, (ASM_ARGS_SP_OFFSET+2+2, sp)
		ldw (2, x), y

		; Clear up stack flags.
		addw sp, #2
		ASM_RETURN
	__endasm;
}
