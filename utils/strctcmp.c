/*******************************************************************************
 *
 * strctcmp.c - Constant-time string comparison implementation
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

#include "common.h"
#include "../utils.h"

// Based upon:
// https://nachtimwald.com/2017/04/02/constant-time-string-comparison-in-c/

int strctcmp(const char *s1, const char *s2) __naked __stack_args {
	(void)s1;
	(void)s2;

	__asm
		; Initialise comparison result (on stack) with non-zero value.
		push #1

		; Load argument pointers to X & Y registers. Will return non-zero if
		; either is null.
		ldw x, (ASM_ARGS_SP_OFFSET+0+1, sp)
		jreq 0004$
		ldw y, (ASM_ARGS_SP_OFFSET+2+1, sp)
		jreq 0004$

		; Reset comparison result to zero.
		clr (1, sp)

	0001$:
		; Compare the current characters from each string pointer by XOR-ing
		; them together. When same, resulting value will be zero, otherwise non-
		; zero. Then OR that with the comparison result - as soon as there is a
		; mismatch, it will become non-zero and stay like that as we continue
		; through the entire string.
		ld a, (x)
		xor a, (y)
		or a, (1, sp)
		ld (1, sp), a

		; If we reached null terminating byte in first string, exit loop.
		; Otherwise, increment its pointer.
		tnz (x)
		jreq 0004$
		incw x

		; Test if reached null terminating byte of second string or not. When
		; non-null, execute an extra no-op to balance the non-taken jump. That
		; is, 2 cycles are always consumed regardless of test outcome (when
		; null, JREQ (2) = 2; when non-null, JREQ (1) + NOP (1) = 2).
		tnz (y)
		jreq 0003$
		nop

	0002$:
		; If not at end of second string, increment its pointer and loop.
		incw y
		jra 0001$

	0003$:
		; If we are at end of second string, in order to balance operations, do
		; a no-op instead to equal the above incrementation in terms of cycles.
		nop
		jra 0001$

	0004$:
		; Return comparison result in X register.
		clrw x
		pop a
		ld xl, a
		ASM_RETURN
	__endasm;
}
