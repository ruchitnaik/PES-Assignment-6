/*
 * main.c - application entry point
 * 
 * Author Howdy Pierce, howdy.pierce@colorado.edu
 */
#include "sysclock.h"
#include "uart.h"
#include "queue.h"
#include "test_queue.h"
#include "hexdump.h"

int main(void)
{
  sysclock_init();

  // TODO: initialize the UART here
  Init_UART0(BAUD_RATE);

  test_cbfifo();

  char str[] = "Welcome to BreakfastSerial!\r\n";
  uint32_t start;
  send_String(str, sizeof(str));
//  start = 0x00;
//  hexdump((void*)start, 128);
//  printf("---New Dump---\r\n");
//  start = 0xa0;
//  hexdump((void*)start, 128);
  // enter infinite loop
  while (1) {
//    __asm volatile ("nop");
	  printf("? ");
	  cmd_accumulate();
  }

  return 0 ;
}
