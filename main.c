#include <stdio.h>
#include <stdlib.h>

#include "Registers.h"
#include "PLL.h" 
#include "GPIO.h"
#include "EdgeTrigger.h"
#include "PWM.h"
#include "UART.h"
#include "ST7735.h"
#include "Drop_the_Bass.h"

void pitch_Handler(uint8_t string_select,uint8_t pitch_select);

int i ;
volatile uint32_t timer = 0,ready = 1,CaptureData = 0,average = 0,tick = 0,averager = 0,counter = 0;
volatile long Counts = 0;

unsigned int string_select = 4; //starts off on string 4
volatile static unsigned int pitch_select[] = {0,0,0,0}; //sets pitch to standard tuning
volatile signed int pitch1 = 0, pitch2 = 0,pitch3 = 0,pitch4 = 0;
unsigned int pitch_increase_flag; // increase pitch flag
unsigned int pitch_decrease_flag; // decrease pitch flag
unsigned long signal_time; //signal from circuit
unsigned long signal_min; //minimum signal the string select will read
unsigned long signal_max; //maximum signal the string select will read
signed long signal_diff; // total difference between real and expected periods
unsigned long pitch_index; //index for fixed signals
unsigned int overflow; // checks if signal and string select are valid
unsigned int timer_value;
unsigned int count;
 
const int sting4S = 7, sting3S = 7,sting2S = 7,sting1S = 7;
char string4[sting4S][2] = {"E","D#","D","C#","C","B"}; 
char string3[sting3S][2] = {"A","G#","G","F#","F","E"};
char string2[sting2S][2] = {"D","C#","C","B","A#","A"};
char string1[sting1S][2] = {"G","F#","F","E","D#","D"};

//  B  F A# E 

double string4P[] = {4100.0,3889.1,3670.7999999999993,3270.3,3086.7999999999997};
double string3P[] = {5500.0,5191.299999999999,4899.9,4365.400000000001,4120.3};
double string2P[] = {7341.5999999999985,6929.6,6540.6,6173.499999999999,5500.0};
double string1P[] = {9798.999999999998,8730.7,8240.699999999999,7778.2,7341.5999999999985};
/*
	e,41 		| d#,38.891 | d,36.708 | C#,34.648 | c,32.703 | b,30.868
	a,55 		| g#,51.913 | g,48.999 | f#,46.249 | f,43.654 | e,41.203
	d,73.416|c#,69.296 	| c,65.406 | b,61.735	 | a#,58.270| a,55
	g,97.99 | f#,92.499 | f,87.307 | e,82.407	 | d#,77.782| d,73.416
*/


int i = 0;
 //System is running at 80Mhz to analyze with the logic analyzer
int main(){
	PLL_Init();
	UART0_Init();
	portF_init();
	EdgeCounter_Init();

	ST7735_InitR(INITR_REDTAB);
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_SetRotation(1);
  ST7735_DrawChar(0,0 ,'4', ST7735_GREEN , ST7735_BLACK, 8 );// Start off with tuning low E
 	ST7735_DrawChar(60,0 ,'E', ST7735_GREEN , ST7735_BLACK, 8);// Start off with tuning low E
  SysTick_Init(800);	 //80MHz sampling at .01ms
 	EnableInterrupts();          
	Header();

 while(1){
	   GPIO_PORTF_DATA_R = 0x00;        
   	}		
}
/*
Systick timer sampling at 100KHz, 1us.
*/
void SysTick_Handler(void){
  GPIO_PORTF_DATA_R = 0x04;        
	if(Counts < 4500){
  Counts = Counts + 1;
	}else{
		Counts = 0; tick = 0;average = 0;averager = 0;
		GPIO_PORTF_DATA_R = 0x00;       // disable PF2
		NVIC_ST_CTRL_R = ~0x07; // disable SysTick with core clock and interrupts
	}
}
/*
Interrupt handler for pitch decrement
*/
void GPIOPortF_Handler(){ 		GPIO_PORTF_DATA_R = 0x02;
switch(string_select){
				case 4:
					pitch4 -= 1;
 					pitch4 = (pitch4+6) % 6;
 				 UART_OutUDec(abs(pitch4));
					UART_OutChar(CR);
					UART_OutChar(LF);
					pitch_Handler(string_select,abs(pitch4));
				break;
				case 3:
					pitch3 -=1;
					pitch3 = (pitch3+6) % 6;
					pitch_Handler(string_select,abs(pitch3));
				break;
				case 2:
					pitch2 -=1;
					pitch2 = (pitch2+6) % 6;
					pitch_Handler(string_select,abs(pitch2));
				break;
				case 1:
					pitch1 -=1;
					pitch1 = (pitch1+6) % 6;
					pitch_Handler(string_select,abs(pitch1));
				break;
			}
				GPIO_PORTF_ICR_R = 0x01;

}

/*
Interrupt handler for Schmitt-Trigger and RC outputs from interface buttons
*/
void GPIOPortD_Handler(){ 		GPIO_PORTF_DATA_R = 0x02;
  switch(GPIO_PORTD_RIS_R){
   case 0x01:
      GPIO_PORTF_DATA_R  = 0x04;
 	    NVIC_ST_CTRL_R = 0x07; // enable SysTick with core clock and interrupts
  	  if(Counts>0){
				tick++;
				average = (average + Counts);
				averager = average/tick; 
				 
	 
				UART_OutUDec(1/(Counts*.00001));
				UART_OutChar('H');
				UART_OutChar('Z');
				UART_OutChar(0x20);
				UART_OutUDec(Counts);
				UART_OutChar('m');
				UART_OutChar('s');
				UART_OutUDec(averager);
				UART_OutChar('A');
				UART_OutChar(' ');
				UART_OutChar(string4[pitch4][0]);
				UART_OutChar(string4[pitch4][1]);
			  UART_OutChar(' ');
 				UART_OutChar('P');
				UART_OutChar(' ');
				UART_OutUDec(abs(string4P[pitch4]-(1/(Counts*.00001))));
				UART_OutChar(' ');
				UART_OutChar('D');
				UART_OutChar(CR);
				UART_OutChar(LF);
				Counts =0;
				}		
       break;
   case 0x02:									//E-String
		 	string_select = 4;
 	    pitch_Handler(4,pitch4);
 			break;
		case 0x04:								//A-String
			string_select = 3;
 			pitch_Handler(3,pitch3);
 			break;
		case 0x08:								//D-String
			string_select = 2;
 			pitch_Handler(2,pitch2);
 			break;
		case 0x40:								//G-String
			string_select = 1;
      pitch_Handler(1,pitch1);
 			break;
		case 0x80:
			switch(string_select){
					case 1:
					pitch1 +=1;
					pitch1 = pitch1 % 6;
					pitch_Handler(string_select,pitch1);
				break;
				case 2:
					pitch2 +=1;
					pitch2 = pitch2 % 6;
					pitch_Handler(string_select,pitch2);
				break;
				case 3:
					pitch3 +=1;
					pitch3 = pitch3 % 6;
					pitch_Handler(string_select,pitch3);
				break;
				case 4:
					pitch4 +=1;
					pitch4 = pitch4 % 6;
					pitch_Handler(string_select,abs(pitch4));
				break;
			}
   		/*UART_OutChar('+');
			UART_OutChar(CR);
			UART_OutChar(LF);*/
 			break;
  }
				GPIO_PORTD_ICR_R = 0xFF;
}

//Graphical layout for the ST7735
void pitch_Handler(uint8_t string_select,uint8_t pitch_select){
		 switch(string_select){
					case 1:
					ST7735_DrawChar(0,0 ,'1', ST7735_GREEN , ST7735_BLACK, 8 ); 
					ST7735_DrawChar(60,0 ,string1[pitch_select][0], ST7735_GREEN , ST7735_BLACK, 8); 
					ST7735_DrawChar(110,0 ,string1[pitch_select][1], ST7735_GREEN , ST7735_BLACK, 8); 
 				break;
				case 2:
					ST7735_DrawChar(0,0 ,'2', ST7735_GREEN , ST7735_BLACK, 8 ); 
					ST7735_DrawChar(60,0 ,string2[pitch_select][0], ST7735_GREEN , ST7735_BLACK, 8); 
					ST7735_DrawChar(110,0 ,string2[pitch_select][1], ST7735_GREEN , ST7735_BLACK, 8); 
 				break;
					case 3:
					ST7735_DrawChar(0,0 ,'3', ST7735_GREEN , ST7735_BLACK, 8 ); 
					ST7735_DrawChar(60,0 ,string3[pitch_select][0], ST7735_GREEN , ST7735_BLACK, 8); 
					ST7735_DrawChar(110,0 ,string3[pitch_select][1], ST7735_GREEN , ST7735_BLACK, 8); 
 				break;
						case 4:
					ST7735_DrawChar(0,0 ,'4', ST7735_GREEN , ST7735_BLACK, 8 ); 
					ST7735_DrawChar(60,0 ,string4[pitch_select][0], ST7735_GREEN , ST7735_BLACK, 8); 
					ST7735_DrawChar(110,0 ,string4[pitch_select][1], ST7735_GREEN , ST7735_BLACK, 8); 
 				break;
			}
			
		}
	
	 
	
 