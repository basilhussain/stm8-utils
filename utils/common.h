/*******************************************************************************
 *
 * common.c - Header for common look-up tables
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

#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>

#ifdef __SDCC_MODEL_LARGE
#define ASM_ARGS_SP_OFFSET 4
#define ASM_RETURN retf
#else
#define ASM_ARGS_SP_OFFSET 3
#define ASM_RETURN ret
#endif

#if defined(REFLECT_LUT)
extern const uint8_t reflect_lut[16];
#endif

#if defined(POP_COUNT_LUT_LARGE)
extern const uint8_t pop_count_lut_large[128];
#elif defined(POP_COUNT_LUT_SMALL)
extern const uint8_t pop_count_lut_small[16];
#endif

#if defined(CTZ_LUT_LARGE)
extern const uint8_t ctz_lut_large[128];
#endif

#if defined(CLZ_LUT_LARGE)
extern const uint8_t clz_lut_large[128];
#endif

#endif // COMMON_H_
