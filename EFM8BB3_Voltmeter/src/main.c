//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This demo demonstrates the ADC on the EFM8BB3 STK using the analog joystick
// or P1.1 as the ADC input.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   ADC0   - 10-bit, VREF = VDD (3.3 V)
//   SPI0   - 1 MHz
//   Timer0 - ADC start of conversion
//   Timer2 - 2 MHz (SPI CS delay)
//   Timer3 - 1 kHz (1 ms tick)
//   P0.2   - Push button
//   P0.6   - SPI SCK
//   P1.0   - SPI MOSI
//   P1.1   - ADC input
//   P1.7   - ADC input/Joystick (analog voltage divider)
//   P2.6   - SPI CS (Active High)
//   P3.4   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB3 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB3 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8BB3 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) The Voltmeter demo should start.
//
// Target:         EFM8BB3
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 15 JUL 2015
//

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "InitDevice.h"
#include "disp.h"
#include "voltmeter.h"
#include <SI_EFM8BB3_Register_Enums.h>
#include "retargetserial.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P3, 4);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (BC_EN, SFR_P2, 2);            // Board Controller Enable
#define BC_DISCONNECTED 0              // 0 = Board Controller disconnected
                                       //     to EFM8 UART pins
#define BC_CONNECTED    1              // 1 = Board Controller connected
                                       //     to EFM8 UART pins




//---------------------------------------------------------------------------
// main() Routine
// --------------------------------------------------------------------------
int main(void)
{

	unsigned char inputcharacter;       // Used to store character from UART

  // Enter default mode
  enter_DefaultMode_from_RESET();

  // Enable all interrupts
  //IE_EA = 1;

  //DISP_Init();



  DISP_EN = DISP_EFM8_DRIVEN;

  BC_EN = BC_CONNECTED;            // Board controller NOT connected to EFM8
                                      // UART pins


  SCON0_TI = 1;                       // This STDIO library requires TI to
                                      // be set for prints to occur

  IE_EA = 1;

  DISP_Init();

  while (1)
  {Voltmeter_main();
		//RETARGET_PRINTF ("\nEnter character: ");
		//inputcharacter = getchar();
		//RETARGET_PRINTF ("\nCharacter entered: %c", inputcharacter);
		//RETARGET_PRINTF ("\n     Value in Hex: %bx", inputcharacter);

  };
}

