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

#include "uart.h"
#include "queue.h"

CBfifo RxQ, TxQ;

int __sys_write(int handle, char* buffer, int count){
	if(buffer == NULL){
		//Return error if null character is passed
		return -1;
	}

	while(cbfifo_full(&TxQ)){
		;			//Wait if transmitter buffer is full
	}

	if(cbfifo_enqueue(&TxQ, buffer, count) != count){
		//Error in enqueue which is propogated further
		return -1;
	}
	if(!(UART0->C2 & UART0_C2_TIE_MASK)){
		UART0->C2 |= UART0_C2_TIE(1);
	}
	return 0;
}

int __sys_readc(void){
	char chatr;
	if(cbfifo_dequeue(&RxQ, &chatr, 1) != 1){
		return -1;
	}
	if((chatr == '\r') || (chatr == '\r')){
		chatr = '\r';
		printf("%c", chatr);
		chatr = '\n';
		printf("%c", chatr);
	}
	else{
		printf("%c", chatr);
	}
	return chatr;
}

void Init_UART0(uint32_t baud_rate){
	uint16_t sbr;

	//Enable clock gating for UART00 and PORTA
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	//Disabling transmitter and receiver before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	//Set UART clock to 24MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);

	//Set pins to UART0 Tx and Rx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2);			//Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2);			//Tx

	//Set bau rate and oversampling ratio
	sbr = (uint16_t)((SYS_CLOCK)/(baud_rate * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr >> 8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE - 1);

	// Disable interrupts for RX active edge and LIN break detect, select two stop bit
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(STOP_CONFIG) | UART_BDH_LBKDIE(0);

	//Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(BIT_MODE) | UART0_C1_PE(PARITY_ENABLE) | UART0_C1_PT(0);
	//Don't invert transit data, do enable interrupt for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0) | UART0_C3_NEIE(0)
				| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	//Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	//Configure to send LSB first
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	//Initializing Queue for further use
	cbfifo_Init(&TxQ);
	cbfifo_Init(&RxQ);

	//Enable UART0 interrupts
	NVIC_SetPriority(UART0_IRQn, 2);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	//Enable receiver interrupts
	UART0->C2 |= UART_C2_RIE(1);

	//Enable UART transmitter and receiver
	UART0->C2 |= UART0_C2_TE(1) | UART0_C2_RE(1);
}


void UART0_IRQHandler(void){
	uint8_t chatr;
	if(UART0->S1 & UART0_S1_RDRF_MASK){
		//Entered here when character is received
		chatr = UART0->D;
		if(!cbfifo_full(&RxQ)){
			//If Rx queue not full
			cbfifo_enqueue(&RxQ, &chatr, 1);
		}
	}

	if((UART0->C2 & UART0_C2_TIE_MASK) && (UART0->C2 & UART0_S1_TDRE_MASK)){
		//Entered here when Tx buffer is empty. Can send another character
		if(cbfifo_dequeue(&TxQ, &chatr, 1) == 1){
			UART0->D = chatr;
		}
		else{
			//Queue empty -> Tx interrupts disabled
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}
}


void send_String(const void* str, size_t count){
	cbfifo_enqueue(&TxQ, str, count);

	//Start transmitting
	if(!(UART0->C2 & UART0_C2_TIE_MASK)){
		UART0->C2 |= UART0_C2_TIE(1);
	}
}


size_t receive_String(void* str, size_t count){
	return cbfifo_dequeue(&RxQ, str, count);
}

void cmd_accumulate(void){
	char acc_buf[640];
	char *ptr_acc = &acc_buf[0];
	uint8_t ch;

	while(ch != '\r'){				//In loop until terminating character is received
		while(cbfifo_empty(&RxQ)){
			;						//Wait if Rx queue is still empty to handle the user input commands
		}

		cbfifo_dequeue(&RxQ, &ch, 1);
		putchar(ch);
		if((ch != '\r') && (ch != '\n')){
			//Not handling backspace here.
			*ptr_acc = (char)ch;
			ptr_acc++;
			*ptr_acc = '\0';		//Adding terminating char at the end, it is overwritten when we receive next char
		}

		if(!(UART0->C2 & UART0_C2_TIE_MASK)){
			UART0->C2 |= UART0_C2_TIE(1);
		}

		if(ch == '\r'){
			ch = '\n';
			printf("\r\n");
			break;
		}
	}

	Process_Message(acc_buf);		//Segmenting the received cmd into token to handle function calls
	ptr_acc = &acc_buf[0];			//Resetting the pointer back to initial location for next accumulation
}
