/*
 * main.c
 */
#include <stdint.h>
#include "tm4c123gh6pm.h"
//////COMENTARIO DE JUAN********VARIABLE GLOBAL VOLATIL QUE SE PUEDE USAR EN LA INTERRUPCION Y EN EL MAIN
volatile uint8_t global;
//////COMENTARIO DE JUAN********DEFINICIONES PARA EL TIMER CON EL HABILITADOR DE INTERRUPCIONES
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
//////COMENTARIO DE JUAN********FUNCIONES DE INTERRUPCION-COPIAR TAL COMO ESTA
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
//////COMENTARIO DE JUAN********INICIALIZA EL TIMER CON INTERRUPCIONES CON UN DADO PERIODO DADO-BUSCAR UN TIMER ADECUADO
// LA RUTINA DE INTERRUPCION (PROGRAMA EN PARALELO) SE EJECUTARA CADA PERIODO
void SysTick_Init(unsigned long period){ // priority 2
	long sr;
	  sr = StartCritical();
	  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
	  NVIC_ST_RELOAD_R = period-1;// reload value
	  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
	  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
	                              // enable SysTick with core clock and interrupts
	  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC+NVIC_ST_CTRL_INTEN;
	  EndCritical(sr);
}
//////COMENTARIO DE JUAN********ESTA PARTE ES REPOIO
void config_UART0 (void){
	// Habilitamos reloj para el UART0
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0;
	// Habilitamos reloj para GPIOA
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	// Inhabilitamos el UART0
	UART0_CTL_R &= ~UART_CTL_UARTEN;
	// Velocidad 9600bps (Fsysclk = 16MHz)
	UART0_IBRD_R = (UART0_IBRD_R & 0xFFFF0000) | 104;
	UART0_FBRD_R = (UART0_FBRD_R & 0xFFFFFFC0) | 11;
	// 8, N, 1, FIFOs habilitados
	UART0_LCRH_R = (UART0_LCRH_R & 0xFFFFFF00) | 0x70;
	// Habilitamos el UART0
	UART0_CTL_R |= UART_CTL_UARTEN;
	// Desactivamos modo analógico en PA0 y PA1
	GPIO_PORTA_AMSEL_R &= ~(0x03);
	// Conectamos UART0 a PA0 y PA1
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)|0x00000011;
	// Activamos funciones alternas en PA0 y PA1
	GPIO_PORTA_AFSEL_R |= 0x03;
	// Activamos funciones digitales en PA0 y PA1
	GPIO_PORTA_DEN_R |= 0x03;
}
//////COMENTARIO DE JUAN********MAS REPOIO
uint8_t rxcar_uart0(void){
	uint8_t temp;
	while(UART0_FR_R & UART_FR_RXFE);
	// Se lee el dato recibido
	temp = UART0_DR_R;
	return temp;
}

//////COMENTARIO DE JUAN********RUTINA DE INTERRUPCION ES EL PROGRAMA QUE SE VA HA REPETIR CADA DETERMINADO PERIODO
/// PARA SU CASO DEBEN USAR TIMERXX_Handler donde XX = numero de timer/letra de subtimer
//USA LA VARIABLE GLOBAL QUE COMPARTE CON MAIN
void SysTick_Handler(void){
  GPIO_PORTF_DATA_R ^= global;       // toggle PF2
}

//////COMENTARIO DE JUAN********EL PROGRAMA NORMAL SOLO ESPERA COMUNICACION SERIAL PARA VARIAR
//LA VARIABLE GLOBAL QUE USA TAMBIEN LA RUTINA DE INTERRUPCION
void main(void) {

	uint8_t dato=0;
	global = 0;
	config_UART0();
	SYSCTL_RCGC2_R |= 0x00000020; // activate port F
	GPIO_PORTF_DIR_R |= 0x06;   // make PF2 output (PF2 built-in LED)
	GPIO_PORTF_AFSEL_R &= ~0x06;// disable alt funct on PF2
	GPIO_PORTF_DEN_R |= 0x06;   // enable digital I/O on PF2
	GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF0FF)+0x00000000;
	GPIO_PORTF_AMSEL_R = 0;     // disable analog functionality on PF
	SysTick_Init(16000000);        // initialize SysTick timer, every 1ms
	EnableInterrupts();         // enable after everything initialized
while(1){
	dato=rxcar_uart0();
	switch(dato)
			{
				case 'A':
				global = 2;
				break;
				case 'S':
				global = 6;
				break;
			}
}
} // fin main()
