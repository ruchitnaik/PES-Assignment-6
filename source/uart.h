/*
 * @File		uart.c
 * @Brief		The file contains functions related to handling the UART on the KL25Z board.
 * 				It also includes the IRQ Hanlder for the UART Tx and Rx and function to bind
 * 				stdio function with the UART using REDLIB library.
 * @Author		Ruchit Naik
 * @Date		03-Nov-2021
 *
 * @Institute	University of Colorado, Boulder
 * @Course		ECEN 8513: Principles of Embedded Software
 *
 * @Attribute
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "MKL25Z4.h"
#include "cli.h"

#define UART_OVERSAMPLE_RATE	15
#define SYS_CLOCK				(24e6)

void send_String(const void* str, size_t count);
size_t receive_String(void* str, size_t count);
void Init_UART0(uint32_t baud_rate);
void cmd_accumulate(void);

#endif /* UART_H_ */
