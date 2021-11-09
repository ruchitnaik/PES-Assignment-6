/*
 * @File		cli.h
 * @Brief		The file contains functions related to handling the UART on the KL25Z board.
 * 				It also includes the IRQ Hanlder for the UART Tx and Rx and function to bind
 * 				stdio function with the UART using REDLIB library.
 * @Author		Ruchit Naik
 * @Date		08-Nov-2021
 *
 * @Institute	University of Colorado, Boulder
 * @Course		ECEN 8513: Principles of Embedded Software
 *
 * @Attribute
 */

#ifndef CLI_H_
#define CLI_H_

#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include "hexdump.h"


void Process_Message(char *input);



#endif /* CLI_H_ */
