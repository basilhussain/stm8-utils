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
}
