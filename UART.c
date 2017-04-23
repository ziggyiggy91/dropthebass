#include "Registers.h"
#include "UART.h"

void UART0_Init(void){ volatile unsigned long delay;           // should be called only once
  SYSCTL_RCGC1_R |= 0x00000001;  // activate UART0
  SYSCTL_RCGC2_R |= 0x00000001;  // activate port A  
  UART0_CTL_R &= ~0x00000001;    // disable UART
  UART0_IBRD_R = 43;     // IBRD = int(80,000,000/(16*115,200)) = int(43.40277778)
  UART0_FBRD_R = 26;     // FBRD = round(0.40277778 * 64) = 26
  UART0_LCRH_R = 0x00000070;  // 8 bit, parity bits, one stop, FIFOs
  UART0_CTL_R |= 0x00000001;     // enable UART

  GPIO_PORTA_AFSEL_R |= 0x03;    // enable alt funct on PA1-0
  GPIO_PORTA_DEN_R |= 0x03;      // configure PA1-0 as UART0
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;
  GPIO_PORTA_AMSEL_R &= ~0x03;  
}

void OutCRLF(void){
  UART_OutChar(CR);
  UART_OutChar(LF);
}

//UART0_rx from terminal 
unsigned char UART_InChar(void){
  while((UART0_FR_R&UART_FR_RXFE) != 0);       
	return (unsigned char)(UART0_DR_R&0xFF);
}

void UART_OutChar(unsigned char data){
  while((UART0_FR_R&UART_FR_TXFF) != 0);
  UART0_DR_R = data;
}

void UART_OutString(char *data){
	while(*data != '%'){
		UART_OutChar(*data);
		data +=1;
	}
}


void UART_OutUDec(unsigned long n){
	if(n >= 10){
		UART_OutUDec(n/10);
		n = n%10;
	}
	UART_OutChar(n + '0');
}
