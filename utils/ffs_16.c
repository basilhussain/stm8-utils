/*******************************************************************************
 *
 * ffs_16.c - 16-bit find first set implementation
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

uint8_t ffs_16(uint16_t value) __stack_args {
	uint8_t idx = 0;
	if(value) idx = ctz_16(value) + 1;
	return idx;

	/*
	// This generates some bizarre, extraneous assembly (see below).
	return (value ? ctz_16(value) + 1 : 0);
	*/

	/*
      000019                         90 _ffs_16:
                                     91 ;	utils\ctz_16.c: 75: return (value ? ctz_16(value) + 1 : 0);
      000019 1E 04            [ 2]   92 	ldw	x, (0x04, sp)
      00001B 27 0F            [ 1]   93 	jreq	00103$
      00001D 89               [ 2]   94 	pushw	x
      00001E 8Ds00r00r00      [ 5]   95 	callf	_ctz_16
      000022 5B 02            [ 2]   96 	addw	sp, #2
      000024 4C               [ 1]   97 	inc	a
      000025 88               [ 1]   98 	push	a      ; <--
      000026 49               [ 1]   99 	rlc	a          ; <--
      000027 4F               [ 1]  100 	clr	a          ; <-- WTF?!?
      000028 A2 00            [ 1]  101 	sbc	a, #0x00   ; <--
      00002A 84               [ 1]  102 	pop	a          ; <--
      00002B 21                     103 	.byte 0x21
      00002C                        104 00103$:
      00002C 4F               [ 1]  105 	clr	a
      00002D                        106 00104$:
                                    107 ;	utils\ctz_16.c: 76: }
      00002D 87               [ 5]  108 	retf
	*/
}
