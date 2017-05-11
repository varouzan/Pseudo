
#include "bsp.h"
#include "pwr.h"

#include "draw.h"
#include "tick.h"
#include "InitDevice.h"
#include <stdio.h>
#include <SI_EFM8BB3_Register_Enums.h>
#include "retargetserial.h"


SI_SBIT(LEDG, SFR_P1, 4);
SI_SBIT (BC_EN, SFR_P2, 2);            // Board Controller Enable
#define BC_DISCONNECTED 0              // 0 = Board Controller disconnected                                       //     to EFM8 UART pins
#define BC_CONNECTED    1              // 1 = Board Controller connected
#define UART_BUFFERSIZE       20

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
volatile uint8_t UART_Buffer[UART_BUFFERSIZE];
volatile uint8_t UART_Buffer_Size = 0;
volatile uint8_t UART_Input_First = 0;
volatile uint8_t Byte = 0;

SI_INTERRUPT (UART0_ISR, UART0_IRQn)
{
	LEDG=1;
	   if (SCON0_RI == 1)
	   {
	      SCON0_RI = 0;                          // Clear interrupt flag

	      if (UART_Input_First < UART_BUFFERSIZE)
	      {
	         UART_Buffer[UART_Input_First] = SBUF0; // Store in array
	         UART_Input_First++;           // Update counter
	      }
	   }

}

SI_INTERRUPT_USING (TIMER4_ISR, TIMER4_IRQn, 1)
{
	    TMR4CN0_TF4H = 0;
}

volatile bool pressed=false;
SI_INTERRUPT(PMATCH_ISR, PMATCH_IRQn)
{
  uint8_t SFRPAGE_save = SFRPAGE;
  	  pressed=true;
}

void main(void)
{
	uint8_t index = 5;
	uint8_t j = 0;
	enter_DefaultMode_from_RESET();
	DISP_Init();
	BC_EN = 0;               // Board controller connected to EFM8
	LEDG=0;
	IE_EA = 1;
	while(true){
		if (UART_Input_First == 19){
			for (j = 0; j<20; j++){
			  UART_Buffer[j]='\0';
			}
			index +=10;
			UART_Input_First =0;
		}
	DrawScreenText(UART_Buffer, index);

	}
}



