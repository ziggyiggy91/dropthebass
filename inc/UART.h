void UART0_Init(void);
unsigned char UART_InChar(void);
void OutCRLF(void);
void UART_OutChar(unsigned char data);
void UART_OutUDec(unsigned long n);
void UART_OutString(char *data);
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F
