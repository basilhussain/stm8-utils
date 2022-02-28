/*******************************************************************************
 *
 * bswap_32.c - 32-bit byte-swap implementation
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

uint32_t bswap_32(const uint32_t value) __naked __stack_args {
	(void)value;

	/*
	| Op      | A  | X    | Y    |
	------------------------------
	|         | -- | AABB | CCDD |
	| swapw x | -- | BBAA | CCDD |
	| swapw y | -- | BBAA | DDCC |
	*/

	__asm
		ldw x, (ASM_ARGS_SP_OFFSET+0, sp)
		ldw y, (ASM_ARGS_SP_OFFSET+2, sp)
		swapw x
		swapw y
		ASM_RETURN
	__endasm;
}
