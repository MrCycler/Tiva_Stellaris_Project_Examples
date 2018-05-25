#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "serialcom.h"

void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;              // 1) disable SysTick during setup
  NVIC_ST_RELOAD_R = 0x00FFFFFF;   // 2) maximum reload value
  NVIC_ST_CURRENT_R = 0;           // 3) any write to current clears it
  NVIC_ST_CTRL_R = 0x00000005;     // 4) enable SysTick with core clock
}

void config_sws(void){
	// activamos la señal de reloj del puerto F
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
	// esperamos a que realmente se active
	while( (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)==0) { }
	// SW1 está conectado al pin PF4 y SW2 al PF0 (Desbloqueamos el SW2)
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R = 0x1F;
	//Configuraciones de puertos
	GPIO_PORTF_DIR_R &= ~(0x11); // PF4 y PF0 pin de entrada
	GPIO_PORTF_AFSEL_R &= ~(0x11); // no usamos función alternativa
	GPIO_PORTF_PUR_R |= 0x11; // activamos resistencia de pull-up en pin PF4
	GPIO_PORTF_DEN_R |= 0x11; // PF4 pin digital
}

void config_leds(void){
	// activamos la señal de reloj del puerto F
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
	// esperamos a que realmente se active
	while( (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)==0) { }
	GPIO_PORTF_DIR_R |= 0x0E; // Configura el bit 1 del puerto F como salida
	GPIO_PORTF_DR8R_R |=0x0E; // se activa el driver de 8 mA en el pin 1.
	GPIO_PORTF_DEN_R |=0x0E; // se activa el pin 1, como salida digital.
	GPIO_PORTF_DATA_R &= ~(0x0E); // apagamos el led
}

void US_trigger(void){
	int n =0;//Funcion para mandar el pulso al trigger del ultrasonico
	GPIO_PORTF_DATA_R |=0x02 ;
	for(n = 0; n < 100; n++);
	GPIO_PORTF_DATA_R  &= ~(0x02) ;
}

int main(void) {
	Setup_UART0();
	unsigned long Elapsed;  // 24-bit time between calls (12.5ns)
	config_sws();
	config_leds();
	while(1){
			SysTick_Init();
			US_trigger();
			while((GPIO_PORTF_DATA_R&0x10)==0);
			while((GPIO_PORTF_DATA_R&0x10)==0x10);
			Elapsed = (0x00FFFFFF-NVIC_ST_CURRENT_R)&0x00FFFFFF; // 24-bit difference
			while(Elapsed!=0)
			{
			 TxCar_UART0((Elapsed%10)+'0');
			 Elapsed=(Elapsed - (Elapsed%10))/10;
			}
			TxMess_UART0("\n\r");

	}
}
