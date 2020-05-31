/*******************************************************************************
 *
 * uart.h - Header file for UART functions
 *
 * Copyright (c) 2020 Basil Hussain
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

#ifndef UART_H_
#define UART_H_

typedef enum {
	UART_BAUD_2400,
	UART_BAUD_9600,
	UART_BAUD_19200,
	UART_BAUD_57600,
	UART_BAUD_115200,
	UART_BAUD_230400,
	UART_BAUD_460800,
	UART_BAUD_921600
} uart_baud_enum_t;

typedef int (*uart_putchar_func_t)(int c);
typedef int (*uart_getchar_func_t)(void);

extern void uart_init(const uart_baud_enum_t baud, uart_putchar_func_t put_func, uart_getchar_func_t get_func);
extern int uart_putchar(int c);
extern int uart_getchar(void);

#endif // UART_H_
