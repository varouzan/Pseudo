
#include "bsp.h"
#include "pwr.h"

#include "draw.h"
#include "tick.h"
#include "InitDevice.h"
#include <stdio.h>
#include <SI_EFM8BB3_Register_Enums.h>
#include "retargetserial.h"


//-----------------------------------------------------------------------------
// Local Definitions
//-----------------------------------------------------------------------------


SI_SBIT(LEDG, SFR_P1, 4);
SI_SBIT (BC_EN, SFR_P2, 2);            // Board Controller Enable
#define BC_DISCONNECTED 0              // 0 = Board Controller disconnected
                                       //     to EFM8 UART pins
#define BC_CONNECTED    1              // 1 = Board Controller connected
#define UART_BUFFERSIZE       20

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
volatile uint8_t UART_Buffer[UART_BUFFERSIZE];
volatile uint8_t UART_Buffer_Size = 0;
volatile uint8_t UART_Input_First = 0;
//volatile uint8_t UART_Output_First = 0;
//volatile uint8_t TX_Ready =1;
volatile uint8_t Byte = 0;

SI_INTERRUPT (UART0_ISR, UART0_IRQn)
{
	LEDG=1;
	   if (SCON0_RI == 1)
	   {
	/*      if( UART_Buffer_Size == 0)  {    // If new word is entered
	         UART_Input_First = 0;    }
*/
	      SCON0_RI = 0;                          // Clear interrupt flag

	//      Byte = SBUF0;                    // Read a character from UART

	      if (UART_Input_First < UART_BUFFERSIZE)
	      {
	         UART_Buffer[UART_Input_First] = SBUF0; // Store in array


	         UART_Input_First++;           // Update counter
	      }
	   }

}

SI_INTERRUPT_USING (TIMER4_ISR, TIMER4_IRQn, 1)
{

	//TMR4CN0_TF4H = 0;


	    TMR4CN0_TF4H = 0;
		//TMR4CN0 &= ~TMR3CN0_TF3H__BMASK;

}
volatile bool pressed=false;
SI_INTERRUPT(PMATCH_ISR, PMATCH_IRQn)
{
  uint8_t SFRPAGE_save = SFRPAGE;
  	  pressed=true;


//  }
  //LEDG=0;
  SFRPAGE = PG2_PAGE;

  //EIE1 &= ~0x02;                     // Disable Port Match interrupts to
                                     // prevent multiple interrupts from
                                     // occurring while the button is
                                     // pressed
  SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Example main()
//-----------------------------------------------------------------------------



void main(void)
{
	//uint8_t Screen [3][20];

	uint8_t index = 5;
	uint8_t j = 0;
	//	uint8_t modeSelected = 1;
	enter_DefaultMode_from_RESET();
	// Enable all interrupts
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



