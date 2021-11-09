/*
 * @File		hexdump.h
 * @Brief		The file contains functions related to handling the hexadecimal dump of the RAM.
 * 				It contains the function to print hexdump based on the given origin on the memory
 * 				and size.
 * @Author		Ruchit Naik
 * @Date		07-Nov-2021
 *
 * @Institute	University of Colorado, Boulder
 * @Course		ECEN 8513: Principles of Embedded Software
 *
 * @Attribute
 */

#ifndef HEXDUMP_H_
#define HEXDUMP_H_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define BYTES_PER_LINE 			 16
#define MAX_HEXDUMP_SIZE 		 640

/**
 * @Function: 	 hexdump()
 * @Parameters:  *loc   - It is the pointer to the location from where the hex dump is to be executed
 * 			 	 nbytes - It is the number of bytes which to be dumped from the location pointed by *loc
 * @Description: The function returns the hex dump from the *loc pointer in the memory till the bytes given
 * 			  	 by the user. It would return the hex dump at in the buffer or array to which *str points to.
 * 			  	 Function would return empty *str in the case of error when *str is not large enough to accomodate
 * 			  	 the entire hex dump.
 */
void hexdump(const void *loc, size_t nbyte);

/**
 * @Function:	 int_to_hexchar()
 * @Parameters:  x - Integer which needs to converted hexdecimal value of the corresponding character
 * @Description: The function converts the given number to hexadecimal character. It returns the char
 * 				 conversion of the interger value given as input to the function.
 */
char int_to_hexchar(int x);

#endif /* HEXDUMP_H_ */
