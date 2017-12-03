/*
 * main.c
 */
#include <stdint.h>
#include "tm4c123gh6pm.h"

void config_sws(void){
	// activamos la señal de reloj del puerto F
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
	// esperamos a que realmente se active
	while( (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)==0) { }
	// SW1 está conectado al pin PF4 y SW2 al PF0
	//////////////////////////////////////////////UNLOCK
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
	GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
	////////////////////////////////////////////////////////////
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


void main(void) {
	uint32_t llave1=1;
	uint32_t llave2=1;
	int n=0;
	config_sws();
	config_leds();
	while(1){
		while((GPIO_PORTF_DATA_R& 0x10) == 0)
		{llave1=0;}//se aprieta sw1 cambia la llave 1
		while((GPIO_PORTF_DATA_R& 0x01) == 0)
		{llave2=0;} //se aprieta sw2 cambia la llave 2
				while(llave1==0)
				{
					for(n = 0; n < 800000; n++);
					GPIO_PORTF_DATA_R  &= ~(0x0E) ;
					GPIO_PORTF_DATA_R |=0x02 ;
					//
					for(n = 0; n < 800000; n++);
					GPIO_PORTF_DATA_R  &= ~(0x0E) ;
					GPIO_PORTF_DATA_R |=0x04 ;
					//
					for(n = 0; n < 800000; n++);
					GPIO_PORTF_DATA_R  &= ~(0x0E) ;
					GPIO_PORTF_DATA_R |=0x08 ;

					while((GPIO_PORTF_DATA_R& 0x01) == 0)
					{llave1=1;GPIO_PORTF_DATA_R  &= ~(0x0D);}
				}
				while(llave2==0)
				{
					for(n = 0; n < 800000; n++);
					GPIO_PORTF_DATA_R  &= ~(0x0E) ;
					GPIO_PORTF_DATA_R |=0x08 ;
					//
					for(n = 0; n < 800000; n++);
					GPIO_PORTF_DATA_R  &= ~(0x0E) ;
					GPIO_PORTF_DATA_R |=0x04 ;
							//
					for(n = 0; n < 800000; n++);
					GPIO_PORTF_DATA_R  &= ~(0x0E) ;
					GPIO_PORTF_DATA_R |=0x02 ;

					while((GPIO_PORTF_DATA_R & 0x10) == 0)
					{llave2=1;GPIO_PORTF_DATA_R  &= ~(0x0E);}
				}
	
	}
}
