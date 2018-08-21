// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// January 15, 2016
//      Jon Valvano and Ramesh Yerraballi

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// Constant declarations to access port registers using
// symbolic names instead of addresses
#define GPIO_PORTE_DATA_R		(*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R		(*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R	(*((volatile unsigned long *)0x40024420))
#define	GPIO_PORTE_PUR_R		(*((volatile unsigned long *)0x40024510))
#define	GPIO_PORTE_DEN_R		(*((volatile unsigned long *)0x4002451C))
#define	GPIO_PORTE_AMSEL_R	(*((volatile unsigned long *)0x40024528))
#define	GPIO_PORTE_PCTL_R		(*((volatile unsigned long *)0x4002452C))
#define SYSCTL_RCGC2_R			(*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOE	0x00000010	//port E Clock Gating Control

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay20ms(unsigned long);
void PortE_Init(void);

// Global variables
unsigned long SW0;	// input from PE0

// ***** 3. Subroutines Section *****

// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).

int main(void){ 
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);  // activate grader and set system clock to 80 MHz
  //unsigned long volatile delay;
	
	// initialization goes here
	PortE_Init();									//Call initiallization of PORTE
  EnableInterrupts();           // enable interrupts for the grader
  while(1){
    Delay20ms(5);
		SW0 = GPIO_PORTE_DATA_R&0x01;		//read PE0 into SW0
		if (SW0) // if the SW0 is pressed (positive logic)
		{
			GPIO_PORTE_DATA_R ^= 0x02;		// toggle PE1 using flip bits
			Delay20ms(1);
		}
		else 			// if the SW0 is not pressed
		{
			GPIO_PORTE_DATA_R |= 0x02;		// set PE1 so the LED is ON
		}
  }
}

// Initialize portE pins for the input and output
// PE0 is input SW0
// PE1 is output to the LED

void PortE_Init(void){
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000010;				// 1/ Turn on E clock
	delay = SYSCTL_RCGC2_R;							// delay
	GPIO_PORTE_AMSEL_R = 0x00;					// 3/ disable analog function
	GPIO_PORTE_PCTL_R = 0x00000000;			// 4/ GPIO clear bit PCTL
	GPIO_PORTE_DIR_R = 0x02;						// 5/ PE0 input, PE1 output
	GPIO_PORTE_AFSEL_R = 0x00;					// 6/ no alternate function
	GPIO_PORTE_PUR_R = 0x00;						//    no pull up resistor
	GPIO_PORTE_DEN_R = 0x03;						// 7/ enable digital pins PE0, PE1
	GPIO_PORTE_DATA_R |= 0x02;					// set PE1 bit in PORTE DATA so the LED is initially ON
}


void Delay20ms(unsigned long time){
	unsigned long k;
	while (time > 0) {
		k = 266666;
		while (k > 0) {
			k = k - 1;
		}
		time = time - 1;
	}
}
