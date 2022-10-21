/*******************************************************************************
 *
 * div_u32.c - 32-bit simultaneous quotient and remainder unsigned division
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

void div_u32(uint32_t x, uint32_t y, div_u32_t *result) __naked __stack_args {
	(void)x;
	(void)y;
	(void)result;

	__asm
		; Offsets and sizes for all stack-held variables and arguments.
		COUNTER_SP_OFFSET = 1
		COUNTER_SP_SIZE = 1
		REMAINDER_SP_OFFSET = COUNTER_SP_OFFSET + COUNTER_SP_SIZE
		REMAINDER_SP_SIZE = 4
		DIVIDEND_SP_OFFSET = REMAINDER_SP_OFFSET + REMAINDER_SP_SIZE + (ASM_ARGS_SP_OFFSET - 1)
		DIVIDEND_SP_SIZE = 4
		DIVISOR_SP_OFFSET = DIVIDEND_SP_OFFSET + DIVIDEND_SP_SIZE
		DIVISOR_SP_SIZE = 4
		RESULT_SP_OFFSET = DIVISOR_SP_OFFSET + DIVISOR_SP_SIZE
		RESULT_SP_SIZE = 2

		; Initialise remainder variable on stack to zero.
		clrw x
		pushw x
		pushw x

		; Initialise loop counter on stack.
		push #32

	0001$:
		; Left shift by one the dividend stack arg value in place, getting the
		; MSb in carry.
		sll (DIVIDEND_SP_OFFSET+3, sp)
		rlc (DIVIDEND_SP_OFFSET+2, sp)
		rlc (DIVIDEND_SP_OFFSET+1, sp)
		rlc (DIVIDEND_SP_OFFSET+0, sp)

		; Load remainder into registers.
		ldw y, (REMAINDER_SP_OFFSET+0, sp)
		ldw x, (REMAINDER_SP_OFFSET+2, sp)

		; Left shift the remainder value by one, putting the MSb from dividend
		; (in carry) into LSb.
		rlcw x
		rlcw y

		; Compare remainder value (still in X&Y) with divisor value.
		; If remainder >= divisor (or divisor < remainder), carry on, otherwise
		; skip ahead.
		cpw x, (DIVISOR_SP_OFFSET+2, sp) ; carry set if divisor > remainder
		ld a, yl
		sbc a, (DIVISOR_SP_OFFSET+1, sp)
		ld a, yh
		sbc a, (DIVISOR_SP_OFFSET+0, sp)
		jrult 0002$

		; Subtract divisor from remainder.
		subw x, (DIVISOR_SP_OFFSET+2, sp)
		rrwa y
		sbc a, (DIVISOR_SP_OFFSET+1, sp)
		rrwa y
		sbc a, (DIVISOR_SP_OFFSET+0, sp)
		rrwa y

		; Set LSb of dividend.
		ld a, (DIVIDEND_SP_OFFSET+3, sp)
		or a, #1
		ld (DIVIDEND_SP_OFFSET+3, sp), a

	0002$:
		; Store remainder value from registers back into stack variable.
		ldw (REMAINDER_SP_OFFSET+0, sp), y
		ldw (REMAINDER_SP_OFFSET+2, sp), x

		; Decrement counter and loop if not yet zero.
		dec (COUNTER_SP_OFFSET, sp)
		jrne 0001$

		; Copy quotient and remainder values to result struct.
		ldw x, (RESULT_SP_OFFSET, sp)
		ldw y, (DIVIDEND_SP_OFFSET+0, sp)
		ldw (x), y
		ldw y, (DIVIDEND_SP_OFFSET+2, sp)
		ldw (2, x), y
		ldw y, (REMAINDER_SP_OFFSET+0, sp)
		ldw (4, x), y
		ldw y, (REMAINDER_SP_OFFSET+2, sp)
		ldw (6, x), y

		; Unwind stack pointer for local variables.
		addw sp, #(COUNTER_SP_SIZE + REMAINDER_SP_SIZE)

		ASM_RETURN
	__endasm;
}
