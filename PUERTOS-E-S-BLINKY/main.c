/*
 * main.c
 */
#include "inc/lm4f120h5qr.h"


void Delay100ms(unsigned long);

int main(void){ unsigned long volatile delay; unsigned char detengo;

  SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
	
	GPIO_PORTF_DATA_R = 0x04;

  while(1){
		detengo = GPIO_PORTF_DATA_R&0x10;
		if (detengo != 0x10 )
    {
			if ((GPIO_PORTF_DATA_R&0x04) == 0x04)
			{GPIO_PORTF_DATA_R &=~ 0x04;}
			else
			 {GPIO_PORTF_DATA_R |= 0x04;}
			Delay100ms(1);
		}
		else
		{GPIO_PORTF_DATA_R = 0x02;
		}

    // body goes here
  }
}

void Delay100ms(unsigned long time){
  unsigned long i;
  while(time > 0){
    i = 1333333;  // this number means 100ms
    while(i > 0){
      i = i - 1;
    }
    time = time - 1; // decrements every 100 ms

  }

}
