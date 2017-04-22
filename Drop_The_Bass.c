#include "UART.h"



void Header(){
UART_OutString("\t\t\t     UART Interface%");UART_OutChar(CR);UART_OutChar(LF);

UART_OutString("*****************************Drop the Bass*****************************%");UART_OutChar(CR);UART_OutChar(LF);
UART_OutString("Description:%");UART_OutChar(CR);UART_OutChar(LF);
UART_OutString("  The device allows a user to tune each string to a specific frequency, %");UART_OutChar(CR);UART_OutChar(LF);
UART_OutString("and the system automatically tunes to a specified setting. %");UART_OutChar(CR);UART_OutChar(LF);
UART_OutString("Ports:%");UART_OutChar(CR);UART_OutChar(LF);
UART_OutString("A: ST7735 SPI%");UART_OutChar(CR);UART_OutChar(LF);
	UART_OutString("D: (PD0)-Bass input,\r\nD: (PD1)-E \r\nD: (PD2)-A \r\nD: (PD3)-D \r\nD: (PD6)-G \r\nD: (PD7)-Pitch Decrement%");UART_OutChar(CR);UART_OutChar(LF);
UART_OutString("F: (PF0)- Pitch Increment%");UART_OutChar(CR);UART_OutChar(LF);
UART_OutString("Team Members:%");UART_OutChar(CR);UART_OutChar(LF);
UART_OutString("  Ismael Garcia-Hardware Engineer%");UART_OutChar(CR);UART_OutChar(LF);
UART_OutString("  Michael Chaffant-Software Engineer%");UART_OutChar(CR);UART_OutChar(LF);
UART_OutString("  Alejandro Felix-Software Engineer%");UART_OutChar(CR);UART_OutChar(LF);
UART_OutString("***********************************************************************%");UART_OutChar(CR);UART_OutChar(LF);
 

}