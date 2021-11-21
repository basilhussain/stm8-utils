/*******************************************************************************
 *
 * ref.h - Header for reference implementations
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

#ifndef REF_H_
#define REF_H_

#include <stdint.h>
#include "utils.h"

extern uint8_t swap_ref(const uint8_t value);
extern uint16_t bswap_16_ref(const uint16_t value);
extern uint32_t bswap_32_ref(const uint32_t value);

extern uint8_t pop_count_8_ref(uint8_t value);
extern uint8_t pop_count_16_ref(uint16_t value);
extern uint8_t pop_count_32_ref(uint32_t value);

extern uint8_t ctz_8_ref(uint8_t value);
extern uint8_t ctz_16_ref(uint16_t value);
extern uint8_t ctz_32_ref(uint32_t value);

extern uint8_t clz_8_ref(uint8_t value);
extern uint8_t clz_16_ref(uint16_t value);
extern uint8_t clz_32_ref(uint32_t value);

extern uint8_t ffs_8_ref(uint8_t value);
extern uint8_t ffs_16_ref(uint16_t value);
extern uint8_t ffs_32_ref(uint32_t value);

extern uint8_t rotate_left_8_ref(uint8_t value, uint8_t count);
extern uint8_t rotate_right_8_ref(uint8_t value, uint8_t count);
extern uint16_t rotate_left_16_ref(uint16_t value, uint8_t count);
extern uint16_t rotate_right_16_ref(uint16_t value, uint8_t count);
extern uint32_t rotate_left_32_ref(uint32_t value, uint8_t count);
extern uint32_t rotate_right_32_ref(uint32_t value, uint8_t count);

extern void div_s16_ref(int16_t x, int16_t y, div_s16_t *result);

#endif // REF_H_
