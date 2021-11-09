/*
 * @File		hexdump.c
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
#include "hexdump.h"


/**
 * @Function:	 int_to_hexchar()
 * @Parameters:  x - Integer which needs to converted hexdecimal value of the corresponding character
 * @Description: The function converts the given number to hexadecimal character. It returns the char
 * 				 conversion of the interger value given as input to the function.
 */
char int_to_hexchar(int ch){
	if (ch >=0 && ch < 10)
		return '0' + ch;
	else if (ch >= 10 && ch < 16)
		return 'A' + ch - 10;
	else
		return '-';
}



/**
 * @Function: 	 hexdump()
 * @Parameters:  *loc   - It is the pointer to the location from where the hex dump is to be executed
 * 			 	 nbytes - It is the number of bytes which to be dumped from the location pointed by *loc
 * @Description: The function returns the hex dump from the *loc pointer in the memory till the bytes given
 * 			  	 by the user. It would return the hex dump at in the buffer or array to which *str points to.
 * 			  	 Function would return empty *str in the case of error when *str is not large enough to accomodate
 * 			  	 the entire hex dump.
 */
void hexdump(const void *loc, size_t nbyte){
	  const uint8_t *ptr = (uint8_t*) loc;
	  const uint8_t *max = (uint8_t*) loc + nbyte;

	  if (nbyte > MAX_HEXDUMP_SIZE) {
		  //Handling error condition if larger hexdump is requested
		  nbyte = MAX_HEXDUMP_SIZE;
	  }

	  while(ptr < max ) {
		    putchar(int_to_hexchar(((uint32_t)(ptr) & 0xF0000000) >> 28));
			putchar(int_to_hexchar(((uint32_t)(ptr) & 0x0F000000) >> 24));
			putchar(int_to_hexchar(((uint32_t)(ptr) & 0x00F00000) >> 20));
			putchar(int_to_hexchar(((uint32_t)(ptr) & 0x000F0000) >> 16));
			putchar('_');
			putchar(int_to_hexchar(((uint32_t)(ptr) & 0x0000F000) >> 12));
			putchar(int_to_hexchar(((uint32_t)(ptr) & 0x00000F00) >>  8));
			putchar(int_to_hexchar(((uint32_t)(ptr) & 0x000000F0) >>  4));
			putchar(int_to_hexchar((uint32_t)(ptr) & 0x0000000F));
			putchar(' ');
			putchar(' ');
			for (int j=0; j < BYTES_PER_LINE && ptr+j < max; j++) {
			  putchar(int_to_hexchar(ptr[j] >> 4));
			  putchar(int_to_hexchar(ptr[j] & 0x0f));
			  putchar(' ');
				}
			ptr += BYTES_PER_LINE;
			putchar('\r');
			putchar('\n');

	  }
}
