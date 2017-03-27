#include <SI_EFM8BB3_Register_Enums.h> //Theo Tronolone main coding

int step;
int opt;
int num;

int main (void)
{
	XBR2 |= 0x40;

	while(1){

		step=0;
		opt=0;
		num=0;

										while(step == 0){
											if(P0_B2 == 0){
												opt++;
													if(opt > 3){
														opt = 0;
													}
											}
											else if(P0_B3 == 0){
												step++;
												return opt;
											}
										}
		switch(opt){
		case 0:
			P3_B1 = 0; P3_B0 = 0;
			P1_B4 = 0; P1_B5 = 0; P1_B6 = 0;
			break;
		case 1:
			P3_B1 = 0; P3_B0 = 1;
			P1_B4 = 1; P1_B5 = 1; P1_B6 = 0;
			break;
		case 2:
			P3_B1 = 1; P3_B0 = 0;
			P1_B4 = 1; P1_B5 = 0; P1_B6 = 1;
			break;
		case 3:
			P3_B1 = 1; P3_B0 = 1;
			P1_B4 = 0; P1_B5 = 1; P1_B6 = 1;
			break;
		}

										while(step == 1){
											if(P0_B2 == 0){
												num++;
													if(num > 15){
														num = 0;
													}
											}
											else if(P0_B3 == 0){
												step++;
												//return num;
											}
										}

		switch(num){
		case 0:
			P1_B0 = 0; P1_B1 = 0; P1_B2 = 0; P1_B3 = 0;
		   break;
		case 1:
			P1_B0 = 0; P1_B1 = 0; P1_B2 = 0; P1_B3 = 1;
		   break;
		case 2:
			P1_B0 = 0; P1_B1 = 0; P1_B2 = 1; P1_B3 = 0;
		   break;
		case 3:
			P1_B0 = 0; P1_B1 = 0; P1_B2 = 1; P1_B3 = 1;
		   break;
		case 4:
			P1_B0 = 0; P1_B1 = 1; P1_B2 = 0; P1_B3 = 0;
		   break;
		case 5:
			P1_B0 = 0; P1_B1 = 1; P1_B2 = 0; P1_B3 = 1;
		   break;
		case 6:
			P1_B0 = 0; P1_B1 = 1; P1_B2 = 1; P1_B3 = 0;
		   break;
		case 7:
			P1_B0 = 0; P1_B1 = 1; P1_B2 = 1; P1_B3 = 1;
		   break;
		case 8:
			P1_B0 = 1; P1_B1 = 0; P1_B2 = 0; P1_B3 = 0;
		   break;
		case 9:
			P1_B0 = 1; P1_B1 = 0; P1_B2 = 0; P1_B3 = 1;
		   break;
		case 10:
			P1_B0 = 1; P1_B1 = 0; P1_B2 = 1; P1_B3 = 0;
		   break;
		case 11:
			P1_B0 = 1; P1_B1 = 0; P1_B2 = 1; P1_B3 = 1;
		   break;
		case 12:
			P1_B0 = 1; P1_B1 = 1; P1_B2 = 0; P1_B3 = 0;
		   break;
		case 13:
			P1_B0 = 1; P1_B1 = 1; P1_B2 = 0; P1_B3 = 1;
		   break;
		case 14:
			P1_B0 = 1; P1_B1 = 1; P1_B2 = 1; P1_B3 = 0;
		   break;
		case 15:
			P1_B0 = 1; P1_B1 = 1; P1_B2 = 1; P1_B3 = 1;
		   break;
		}

										while(step == 2){
											if(P0_B3 == 0){
												break;
											}
										}


   }
}
