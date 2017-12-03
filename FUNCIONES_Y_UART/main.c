/*
 * main.c
 */
#include "inc/lm4f120h5qr.h"
#include "utils/uartstdio.h"


// ***** 2. Global Declarations Section *****
void UART_Init(void);

//------------UART_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
unsigned char UART_InChar(void);

//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(unsigned char data);


// FUNCTION PROTOTYPES: Each subroutine defined
unsigned long Calc_Area(unsigned long l, unsigned long w);
void EnableInterrupts(void);  // Enable interrupts

// ***** 3. Subroutines Section *****
int main (void) {
  unsigned long length, width, area;
	
  UART_Init();    // initialize UART for printing
  printf("\nThis program calculates areas of rectangular rooms.\n");

  while(1) {
    printf("\nGive length: "); scanf("%ld", &length);  // Get input
    printf("\nGive width: ");  scanf("%ld", &width);   // Get input
    area = Calc_Area(length, width);
    printf("\nArea of the room = %ld\n",  area);
  }
}

// Computes the Area of a rectangular room whose sides are given
// Input:  the length (l) and width (w) are the dimensions of a rectangle
// Output: the area of the rectangular room
// Notes:  Return a result of zero if
//    the length is less than 3,
//    the width is less than 3,
//    the length is greater than 20 or
//    the width is greater than 20.
unsigned long Calc_Area(unsigned long l, unsigned long w) {
  unsigned long result;
  if((l<3)||(l>20)||(w<3)||(w>20))
	{result = 0;}
	else
	{result = l *w;}
// Put your Lab 5 code here

  return(result);
}
void UART_Init(void){
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0; // activate UART0
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // activate port A
  UART0_CTL_R &= ~UART_CTL_UARTEN;      // disable UART
  UART0_IBRD_R = 43;                    // IBRD = int(80,000,000 / (16 * 115200)) = int(43.402778)
  UART0_FBRD_R = 26;                    // FBRD = round(0.402778 * 64) = 26
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  UART0_CTL_R |= UART_CTL_UARTEN;       // enable UART
  GPIO_PORTA_AFSEL_R |= 0x03;           // enable alt funct on PA1-0
  GPIO_PORTA_DEN_R |= 0x03;             // enable digital I/O on PA1-0
                                        // configure PA1-0 as UART
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;
  GPIO_PORTA_AMSEL_R &= ~0x03;          // disable analog functionality on PA
}

//------------UART_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
unsigned char UART_InChar(void){
  while((UART0_FR_R&UART_FR_RXFE) != 0);
  return((unsigned char)(UART0_DR_R&0xFF));
}
//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(unsigned char data){
  while((UART0_FR_R&UART_FR_TXFF) != 0);
  UART0_DR_R = data;
}



// Print a character to UART0.
int fputc(int ch, FILE *f){
  if((ch == 10) || (ch == 13) || (ch == 27)){
    UART_OutChar(13);
    UART_OutChar(10);
    return 1;
  }
  UART_OutChar(ch);
  return 1;
}
// input from UART, return data.
int fgetc(FILE *f){
	char ch;
	ch = UART_InChar();
	UART_OutChar(ch);
  return ch;
}
// Function called when file error occurs.
int ferror(FILE *f){
  /* Your implementation of ferror */
  return EOF;
}

