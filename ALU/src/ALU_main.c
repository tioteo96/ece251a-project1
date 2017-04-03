#include <SI_EFM8BB3_Register_Enums.h>
#include "InitDevice.h"

//this was used just to test out the code
void null (void);
void red (void);
void blue (void);
void green (void);
void all (void);

void binarynum (int);  //input number
void binaryopt (int);  //3:add//2:OR//1:AND//0:ALL 0
void delay (void); //delay fcn (somewhat like a clock)
void twoscomp (int); //for subtraction (but only up to -8 because starting from -9, more bits are required

extern void WDT_0_enter_DefaultMode_from_RESET(void); //watchdog

int num; //input number
int opt; //op-code
int i; //needed for the delay fcn

SI_SBIT(N0, SFR_P1, 0);
SI_SBIT(N1, SFR_P1, 1);
SI_SBIT(N2, SFR_P1, 2);
SI_SBIT(N3, SFR_P1, 3);
//assigning new names were considered unnecessary so did only for the 4 input numbers

int main(void) {
	enter_DefaultMode_from_RESET(); //the basic setting that allows the output pins to be functional
	WDT_0_enter_DefaultMode_from_RESET(); //watchdog

	XBR2 |= 0x40;// allows us to switch the value of the pins
	P0 = 0;// in order to ensure that all outputs are 0 before entering the loop
	P1 = 0;
	P2 = 0;
	P3 = 0;

	while (1) {

		null();//just to turn off the annoying led
		binaryopt(3);
		
		if(P0_B3 == 0 || P0_B2 == 0){
			if(P0_B3 == 0){
				binarynum(1);
				delay();
			}
			else if(P0_B2 == 0){
				twoscomp(1);
				delay();
			}
			P0_B7 = 1;
		}
		else{
			P0_B7 = 0;
		}
	}
}

void null (void){
	P1_B4 = 1; P1_B5 = 1; P1_B6 = 1;
}
void red (void){
	P1_B4 = 1; P1_B5 = 1; P1_B6 = 0;
}
void blue (void){
	P1_B4 = 1; P1_B5 = 0; P1_B6 = 1;
}
void green (void){
	P1_B4 = 0; P1_B5 = 1; P1_B6 = 1;
}
void all (void){
	P1_B4 = 0; P1_B5 = 0; P1_B6 = 0;
}

void delay (void){
	for(i = 0; i < 30000; i++){

	}
}

void binarynum (int a){
	switch(a){
	case 0:
		N0 = 0; N1 = 0; N2 = 0; N3 = 0;
	    break;
	case 1:
		N0 = 0; N1 = 0; N2 = 0; N3 = 1;
		break;
	case 2:
		N0 = 0; N1 = 0; N2 = 1; N3 = 0;
		break;
	case 3:
		N0 = 0; N1 = 0; N2 = 1; N3 = 1;
		break;
	case 4:
		N0 = 0; N1 = 1; N2 = 0; N3 = 0;
		break;
	case 5:
		N0 = 0; N1 = 1; N2 = 0; N3 = 1;
		break;
	case 6:
		N0 = 0; N1 = 1; N2 = 1; N3 = 0;
		break;
	case 7:
		N0 = 0; N1 = 1; N2 = 1; N3 = 1;
		break;
	case 8:
		N0 = 1; N1 = 0; N2 = 0; N3 = 0;
		break;
	case 9:
		N0 = 1; N1 = 0; N2 = 0; N3 = 1;
		break;
	case 10:
		N0 = 1; N1 = 0; N2 = 1; N3 = 0;
		break;
	case 11:
		N0 = 1; N1 = 0; N2 = 1; N3 = 1;
		break;
	case 12:
		N0 = 1; N1 = 1; N2 = 0; N3 = 0;
		break;
	case 13:
		N0 = 1; N1 = 1; N2 = 0; N3 = 1;
		break;
	case 14:
		N0 = 1; N1 = 1; N2 = 1; N3 = 0;
		break;
	case 15:
		N0 = 1; N1 = 1; N2 = 1; N3 = 1;
		break;
	}
}

void binaryopt (int b){
	switch(b){
	case 0:
		P0_B0 = 0; P0_B1 = 0;
		break;
	case 1:
		P0_B0 = 0; P0_B1 = 1;
		break;
	case 2:
		P0_B0 = 1; P0_B1 = 0;
		break;
	case 3:
		P0_B0 = 1; P0_B1 = 1;
		break;
	}
}

void twoscomp (int c){
	switch(c){
	case 0:
		N0 = 0; N1 = 0; N2 = 0; N3 = 0;
		break;
	case 1:
		N0 = 1; N1 = 1; N2 = 1; N3 = 1;
		break;
	case 2:
		N0 = 1; N1 = 1; N2 = 1; N3 = 0;
		break;
	case 3:
		N0 = 1; N1 = 1; N2 = 0; N3 = 1;
		break;
	case 4:
		N0 = 1; N1 = 1; N2 = 0; N3 = 0;
		break;
	case 5:
		N0 = 1; N1 = 0; N2 = 1; N3 = 1;
		break;
	case 6:
		N0 = 1; N1 = 0; N2 = 1; N3 = 0;
		break;
	case 7:
		N0 = 1; N1 = 0; N2 = 0; N3 = 1;
		break;
	case 8:
		N0 = 1; N1 = 0; N2 = 0; N3 = 0;
		break;
}

extern void WDT_0_enter_DefaultMode_from_RESET(void) {
//the watchdog was necessary since it allowed us to avoid from the while loop resetting by itself
	SFRPAGE = 0x00;
	WDTCN = 0xDE; 
	WDTCN = 0xAD; 
}
