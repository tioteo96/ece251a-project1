/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// voltmeter.c
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "tick.h"
#include "disp.h"
#include "render.h"
#include "voltmeter.h"
#include "joystick.h"
#include "adc.h"
#include "thinfont.h"
#include "dpad_n.h"
#include "dpad_e.h"
#include "dpad_s.h"
#include "dpad_w.h"
#include "dpad_c.h"
#include "dpad_nw.h"
#include "dpad_ne.h"
#include "dpad_sw.h"
#include "dpad_se.h"
#include "dpad_bold_n.h"
#include "dpad_bold_e.h"
#include "dpad_bold_s.h"
#include "dpad_bold_w.h"
#include "dpad_bold_c.h"
#include "dpad_bold_nw.h"
#include "dpad_bold_ne.h"
#include "dpad_bold_sw.h"
#include "dpad_bold_se.h"




#include <SI_EFM8BB3_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
// Configurator set for HFOSC0/8
#define SYSCLK             3062000

// Configurator set for timer overflow every 100 ms / 10 Hz.
#define LED_TOGGLE_RATE           100  // LED toggle rate in milliseconds
// if LED_TOGGLE_RATE = 1, the LED will
// be on for 1 millisecond and off for
// 1 millisecond

SI_SBIT (LED_RED, SFR_P1, 6);// red LED
SI_SBIT (LED_BLUE, SFR_P1, 5);// blue LED
SI_SBIT (LED_GREEN, SFR_P1, 4);// green LED

SI_SBIT(BTN0, SFR_P0, 2);                 // P0.2 BTN0
SI_SBIT(BTN1, SFR_P0, 3);                 // P0.3 BTN1
///////////////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////////////

SI_SEGMENT_VARIABLE_SEGMENT_POINTER(west_bits,      uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(east_bits,      uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(north_bits,     uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(south_bits,     uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(center_bits,    uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(northwest_bits, uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(northeast_bits, uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(southwest_bits, uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(southeast_bits, uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);

// Generic line buffer
SI_SEGMENT_VARIABLE(Line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);

// ADC input: joystick or external
bool adcInput;
#define ADCINPUT_JOYSTICK 0
#define ADCINPUT_EXTERNAL 1

// Flag to update the ADC input text on the screen.
uint8_t updateAdcInput;
#define UPDATEADCINPUT_NO  0
#define UPDATEADCINPUT_YES 1

///////////////////////////////////////////////////////////////////////////////
// Supporting Functions
///////////////////////////////////////////////////////////////////////////////
char timeStr[7];
static uint16_t time = 0;
uint8_t lapcount = 0;
uint8_t lapnow = 0;
uint8_t resetopt = 0;
//-----------------------------------------------------------------------------
// convertMillivoltToVolt
//-----------------------------------------------------------------------------
//
// Converts mV string to V string with units.
// e.g. '3300' to '3.300 V'
//
// mVolt - 0 to 3300 mV
// voltStr  - string with V info, length is 9 (added decimal point, space, and unit)
//
void convertMillivoltToVolt(uint16_t mVolt, char * voltStr)
{
  int8_t pos = 7;

  // Work backwards generating the string
  // Start with null-terminator
  // followed by V symbol  

  voltStr[pos--] = '\0';
  voltStr[pos--] = 's';
  
  // followed by thousandths digit
  // followed by hundredths digit
  // followed by tenths digit
  // followed by the decimal point at pos=1
  // followed by the ones digit
  for (; pos >= 0; pos--)
  {
    // Pos = 1 is the location of the decimal point
    if (pos == 3)
    {
      voltStr[pos] = '.';
    }
    else
    {
      // Get the right-most digit from the number
      // and convert to ascii
      voltStr[pos] = (mVolt % 10) + '0';
      
      // Shift number by 1 decimal place to the right
      mVolt /= 10; 
    }
  }
}

//-----------------------------------------------------------------------------
// drawScreenText
//-----------------------------------------------------------------------------
//
// Show one line of text on the screen
//
// str - pointer to string text (0 - 21 characters) to display
// rowNum - row number of the screen (0 - 127)
// fontScale - font scaler (1 - 4)
//
void drawScreenText(SI_VARIABLE_SEGMENT_POINTER(str, char, RENDER_STR_SEG), uint8_t rowNum, uint8_t fontScale)
{
  uint8_t i, j;

  for (i = 0; i < FONT_HEIGHT; i++)
  {
    RENDER_ClrLine(Line);
    RENDER_Large_StrLine(Line, 4, i, (SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))str, fontScale);

    for (j = 1; j < fontScale + 1; j++)
    {
      DISP_WriteLine(rowNum + (i * fontScale) + j, Line);
    }
  }
}



SI_INTERRUPT (TIMER0_ISR, TIMER0_IRQn)
{
	static uint16_t counter = 0;

	counter++;

	if(counter == LED_TOGGLE_RATE)
	{
		//LED_RED = !LED_RED;                    // Toggle the LED
		time++;
		counter = 0;
	}
}



SI_INTERRUPT (INT1_ISR, INT1_IRQn)
{
	//LED_BLUE = !LED_BLUE;
	if(BTN1 == 0){
			lapcount++; //increase lapcount to tell drawLap() routine to continue
	}
}

SI_INTERRUPT (INT0_ISR, INT0_IRQn)
{
	//LED_GREEN = !LED_GREEN;
	if(BTN0 == 0){
			resetopt++;
	}
}
///////////////////////////////////////////////////////////////////////////////
// Driver Function

void NewLap(){
	if(lapnow != lapcount){
		convertMillivoltToVolt(time*5, timeStr);
		drawScreenText(timeStr, 50 + (10*lapnow), FONT_SCALE_DEFAULT);
	}
	lapnow = lapcount;
}

void ResetLap(){
		drawScreenText(" ", 50, FONT_SCALE_DEFAULT);
		drawScreenText(" ", 60, FONT_SCALE_DEFAULT);
		drawScreenText(" ", 70, FONT_SCALE_DEFAULT);
		drawScreenText(" ", 80, FONT_SCALE_DEFAULT);
		drawScreenText(" ", 90, FONT_SCALE_DEFAULT);
		drawScreenText(" ", 100, FONT_SCALE_DEFAULT);
		drawScreenText(" ", 120, FONT_SCALE_DEFAULT);
		drawScreenText(" ", 130, FONT_SCALE_DEFAULT);
		drawScreenText(" ", 140, FONT_SCALE_DEFAULT);
		drawScreenText(" ", 150, FONT_SCALE_DEFAULT);
		drawScreenText(" ", 160, FONT_SCALE_DEFAULT);
		drawScreenText(" ", 170, FONT_SCALE_DEFAULT);
		drawScreenText(" ", 180, FONT_SCALE_DEFAULT);


		lapnow = 0;
		lapcount = 0;
}

extern void start(void);

void Voltmeter_main(void)
{
  uint32_t adcVoltage = 0;

  // Initial ADC input is joystick
  adcInput = ADCINPUT_JOYSTICK;
  updateAdcInput = UPDATEADCINPUT_YES;

  start();
  time = 0;


  while(1){

  drawScreenText("Theo & John",       10, FONT_SCALE_DEFAULT);
  drawScreenText("Stopwatch Project", 20, FONT_SCALE_DEFAULT);

  convertMillivoltToVolt(time*5, timeStr);
  drawScreenText(timeStr, 30, 2);


  if(P0_B2 == 0){
	ResetLap();
  }

  NewLap();

  }
}
