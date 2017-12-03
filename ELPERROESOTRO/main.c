#include <stdint.h>
#include "tm4c123gh6pm.h"

void main(void) {
	// activamos la señal de reloj del puerto F
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; //A0 //B1 //C2 //D3 //E4
	// esperamos a que realmente se active
			while( (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)==0)
			{
			/* hay que esperar ... */
			}
	// ahora configuramos el pin PF1
		GPIO_PORTF_DIR_R |= 0x02; // Configura el pin PF1 como salida /0/0/0/0/0/0/1/0/ ->0x02
		GPIO_PORTF_DR8R_R |=0x02; // se activa el driver de 8 mA en el pin PF1.
		GPIO_PORTF_DEN_R |=0x02; // se activa el pin PF1 como salida digital.
			while(1)
			{
				GPIO_PORTF_DATA_R ^=0X02; // salida del dato en el pin PF1, como pulsos.
			}
} // fin main()
