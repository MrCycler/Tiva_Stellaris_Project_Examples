/*
 PROGRAMA: PULSACIONES EN LED
 Autor: RKZ
 DESCRIPCION: Realiza la pulsación repetitiva del led PF1 en color morado
 */
#include <stdint.h>
#include "tm4c123gh6pm.h"

void main(void) {
	int n=0;
	// activamos la señal de reloj del puerto F
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
	// esperamos a que realmente se active
	while( (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)==0){}
	// ahora configuramos el pin PF1
	GPIO_PORTF_DIR_R |= 0x06; // Configura el pin PF1 como salida
	GPIO_PORTF_DR8R_R |=0x06; // se activa el driver de 8 mA en el pin PF1.
	GPIO_PORTF_DEN_R |=0x06; // se activa el pin PF1 como salida digital.
	while(1){
		GPIO_PORTF_DATA_R ^=0X06; // salida del dato en el pin PF1, como pulsos.
		for(n = 0; n < 900000; n++);//espera un tiempo para activar o desactivar la luz en el led
}
}
