#include <SI_EFM8BB3_Register_Enums.h>

int time = 5;
int a;
int b;

int main (void)
{

	XBR2 |= 0x40;




	 while (1)
    {
		 if(P0_B2 == 0){
		 	    	    	time++;
		 	    	    }
		 if(P0_B3 == 0){
		 	    	    	time--;
		 	    	    }

    	P1_B4 = 1;
    	for(a = 1;a < (10000 + (500*time));a++){
    	}
    	P1_B4 = 0;
    	for(b = 1;b < (10000 + (500*time));b++){
    	}
    };

}