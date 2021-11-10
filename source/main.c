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
  send_String(str, sizeof(str));
  // enter infinite loop
  while (1) {
	  printf("? ");
	  cmd_accumulate();
  }
  return 0 ;
}
