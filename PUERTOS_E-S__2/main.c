/*
 * main.c
 */
#include "inc/lm4f120h5qr.h"

unsigned long SW1,SW2;  // input from PF4,PF0
unsigned long Out;      // outputs to PF3,PF2,PF1 (multicolor LED)

//   Function Prototypes
void PortF_Init(void);
void Delay(void);



// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){
  PortF_Init();        // Call initialization of port PF4, PF3, PF2, PF1, PF0

  while(1){
    SW1 = GPIO_PORTF_DATA_R&0x10;     // read PF4 into SW1
    SW2 = GPIO_PORTF_DATA_R&0x01;     // read PF0 into SW2
    if(SW1&&SW2){                     // both pressed
      GPIO_PORTF_DATA_R = 0x00;       // LED is blue
    } else{
      if(SW1&&(!SW2)){                // just SW1 pressed
        GPIO_PORTF_DATA_R = 0x08;     // LED is red
      } else{
        if((!SW1)&&SW2){              // just SW2 pressed
          GPIO_PORTF_DATA_R = 0x02;   // LED is green
        }else{                        // neither switch
          GPIO_PORTF_DATA_R = 0x04;   // LED is off
        }
      }
    }
  }
}
// Subroutine to initialize port F pins for input and output
// PF4 and PF0 are input SW1 and SW2 respectively
// PF3,PF2,PF1 are outputs to the LED
// Inputs: None
// Outputs: None
// Notes: These five pins are connected to hardware on the LaunchPad
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0
}
// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06


