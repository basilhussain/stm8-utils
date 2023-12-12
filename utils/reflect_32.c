/*******************************************************************************
 *
 * reflect_32.c - 32-bit reversal/reflection implementation
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

uint32_t reflect_32(uint32_t value) __naked __stack_args {
	(void)value;

	// For return value/arg: 0xAABBCCDD
	// y = 0xAABB (yh = 0xAA, yl = 0xBB)
	// x = 0xCCDD (xh = 0xCC, xl = 0xDD)

#ifdef REFLECT_LUT

	// 66 cycles (exc. return), 116+16=132 bytes
	__asm
		.macro reflect_32_rev_swap_nibbles stk_off
			; Load byte from given stack offset, mask upper nibble, swap it to
			; lower position, and put into X reg.
			ld a, (ASM_ARGS_SP_OFFSET+stk_off, sp)
			and a, #0xF0
			swap a
			clrw x
			ld xl, a

			; Re-load byte from given stack offset, mask lower nibble, and put
			; into Y reg.
			ld a, (ASM_ARGS_SP_OFFSET+stk_off, sp)
			and a, #0x0F
			clrw y
			ld yl, a

			; Lookup lower nibble reflected value in table and swap it into
			; upper nibble of result. Then do same for upper nibble and put into
			; lower nibble of result. Copy back to given stack offset.
			ld a, (_reflect_lut, y)
			swap a
			or a, (_reflect_lut, x)
			ld (ASM_ARGS_SP_OFFSET+stk_off, sp), a
		.endm

		; Load MSW and LSW from argument value to Y & X regs, swap the bytes
		; within them, then transpose the words when we load back to stack.
		ldw y, (ASM_ARGS_SP_OFFSET+0, sp)
		ldw x, (ASM_ARGS_SP_OFFSET+2, sp)
		swapw x
		swapw y
		ldw (ASM_ARGS_SP_OFFSET+0, sp), x
		ldw (ASM_ARGS_SP_OFFSET+2, sp), y

		; Using the LUT, reverse and swap the individual nibbles of each stack
		; byte.
		reflect_32_rev_swap_nibbles 0
		reflect_32_rev_swap_nibbles 1
		reflect_32_rev_swap_nibbles 2
		reflect_32_rev_swap_nibbles 3

		; Copy resulting value from stack to X and Y registers for return value.
		ldw y, (ASM_ARGS_SP_OFFSET+0, sp)
		ldw x, (ASM_ARGS_SP_OFFSET+2, sp)
		ASM_RETURN
	__endasm;

#else

	// unrolled 256 cycles (exc. return), 353 bytes
	__asm
		.macro reflect_32_shift
			srl (ASM_ARGS_SP_OFFSET+0, sp)
			rrc (ASM_ARGS_SP_OFFSET+1, sp)
			rrc (ASM_ARGS_SP_OFFSET+2, sp)
			rrc (ASM_ARGS_SP_OFFSET+3, sp)
			rlcw x
			rlcw y
		.endm

#ifdef REFLECT_UNROLLED

		.rept 32
			reflect_32_shift
		.endm

#else

		ld a, #32

	0001$:
		reflect_32_shift
		dec a
		jrne 0001$

#endif

		; The X and Y registers contain return value.
		ASM_RETURN
	__endasm;

#endif
}
