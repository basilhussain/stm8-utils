/*******************************************************************************
 *
 * reflect_16.c - 16-bit reversal/reflection implementation
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

uint16_t reflect_16(uint16_t value) __naked __stack_args {
	(void)value;

#ifdef REFLECT_LUT

	// 33 cycles (exc. return), 58+16=74 bytes
	__asm
		.macro reflect_16_rev_swap_nibbles stk_off
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

		; Load word from argument value to X register, swap the bytes within it,
		; then load back to stack.
		ldw x, (ASM_ARGS_SP_OFFSET+0, sp)
		swapw x
		ldw (ASM_ARGS_SP_OFFSET+0, sp), x

		; Using the LUT, reverse and swap the individual nibbles of each stack
		; byte.
		reflect_16_rev_swap_nibbles 0
		reflect_16_rev_swap_nibbles 1

		; Copy resulting value from stack to X register for return value.
		ldw x, (ASM_ARGS_SP_OFFSET+0, sp)
		ASM_RETURN
	__endasm;

#else

	// unrolled 64 cycles (exc. return), 81 bytes
	__asm
		.macro reflect_16_shift
			srl (ASM_ARGS_SP_OFFSET+0, sp)
			rrc (ASM_ARGS_SP_OFFSET+1, sp)
			rlcw x
		.endm

#ifdef REFLECT_UNROLLED

		.rept 16
			reflect_16_shift
		.endm

#else

		ld a, #16

	0001$:
		reflect_16_shift
		dec a
		jrne 0001$

#endif

		; Return value is already in X reg.
		ASM_RETURN
	__endasm;

#endif
}
