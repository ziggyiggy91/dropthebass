#include "Registers.h"
#include "EdgeTrigger.h"


void EdgeCounter_Init(void){	volatile unsigned long delay;
                         
  SYSCTL_RCGC2_R |= 0x00000028; // (a) activate clock for port D & F
  delay = SYSCTL_RCGC2_R;           // delay 
	GPIO_PORTD_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
 	GPIO_PORTD_CR_R = 0xFF;         // allow changes to PF4,0
  GPIO_PORTD_AMSEL_R = 0;       //     disable analog functionality on PF	// (f)disable interrupt on PD2
  GPIO_PORTD_PCTL_R &= ~0xFFFFFFFF; // configure PF4 as GPIO
 	GPIO_PORTD_DIR_R &= ~0xFF;    // (c) make PD2 in (built-in button)
  GPIO_PORTD_AFSEL_R &= ~0xFF;  //     disable alt funct on PD2
  GPIO_PORTD_DEN_R |= 0xFF;     //     enable digital I/O on PF4   
  GPIO_PORTD_IS_R &= ~0xFF;     // (d) PD2 is edge-sensitive 
  GPIO_PORTD_IBE_R &=  ~0xFF;    //     PD2 is not both edges
	GPIO_PORTD_PUR_R |=  0x00;     //    disable weak pull-up on PD0
  GPIO_PORTD_IEV_R &= 0xFF;    //     PD2 falling edge event
  GPIO_PORTD_ICR_R = 0xFF;      // (e) clear flag0,flag1
  GPIO_PORTD_IM_R |= 0xFF;      // (f) arm interrupt on PD2
	NVIC_PRI0_R = (NVIC_PRI0_R&0x0FFFFFFF)|0x00000000; // (g) priority 0

	//PORTF
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x01;         // allow changes to PF4,0
  GPIO_PORTF_AMSEL_R = 0;       //     disable analog functionality on PF	// (f)disable interrupt on PD2
  GPIO_PORTF_PCTL_R &= ~0xFFFFFFFF; // configure PF4 as GPIO
 	GPIO_PORTF_DIR_R &= ~0x01;    // (c) make PD2 in (built-in button)
  GPIO_PORTF_AFSEL_R &= ~0x01;  //     disable alt funct on PD2
  GPIO_PORTF_DEN_R |= 0x01;     //     enable digital I/O on PF4   
  GPIO_PORTF_IS_R &= ~0x01;     // (d) PD2 is edge-sensitive 
  GPIO_PORTF_IBE_R &=  ~0x01;    //     PD2 is not both edges
	GPIO_PORTF_PUR_R |=  0x00;     //    disable weak pull-up on PD0
  GPIO_PORTF_IEV_R &= 0x01;    //     PD2 falling edge event
  GPIO_PORTF_ICR_R = 0x01;      // (e) clear flag0,flag1
  GPIO_PORTF_IM_R |= 0x01;      // (f) arm interrupt on PD2
	
 	NVIC_PRI7_R = (NVIC_PRI7_R&0x000FFFFF)|0x00100000; // (g) priority 1

  NVIC_EN0_R = 0x40000008;      // (h) enable interrupt 8 in NVIC
 }

void PeriodicTimer(double period){
	SYSCTL_RCGCTIMER_R|=0x0008;
	TIMER3_CTL_R  = 0x00000000;
	TIMER3_CFG_R  = 0x00000000;
	TIMER3_TAMR_R = 0x00000002;
 	TIMER3_TAILR_R =(period*1000) - 1; //.1 ms for 10mhz clock
	TIMER3_TAPR_R = 0;
	TIMER3_ICR_R = 0x00000001;
	TIMER3_IMR_R = 0x00000001; 
	NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0X40000000;
	NVIC_EN1_R = 1<<(35-32);
	TIMER3_CTL_R |= 0x00000001;
 }

 
void InterruptTimer(double period){
	SYSCTL_RCGCTIMER_R|=0x0008;
	TIMER3_CTL_R  = 0x00000000;//Timer Disabled
	TIMER3_CFG_R  = 0x00000000;
	TIMER3_TAMR_R = 0x000000A2;        //TASNAPS | TAMIE , One-shot Timer mode
 	TIMER3_TAILR_R =(period) - 1; //.1 ms for 10mhz clock
	TIMER3_TAPR_R = 0;
	TIMER3_ICR_R = 0x00000011;
	TIMER3_IMR_R = 0x00000013; 
	NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0X40000000;
	NVIC_EN1_R = 1<<(35-32);
	TIMER3_CTL_R |= 0x00000001; //TAMCINT TIMERA Match Interrupt clear, Timer Enable
 }

 void SysTick_Init(long period){
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = (period) - 1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2          
  NVIC_ST_CTRL_R = ~0x07; // enable SysTick with core clock and interrupts
  // enable interrupts after all initialization is finished
}

 /*
 Use this register to check status of timer
 if (TIMER3_MIS_R&0x00000010 == 1)
 
 */