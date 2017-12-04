/*
 PROGRAMA: ELIMINA COLOR DE SECUENCIA
 Autor: RKZ
 DESCRIPCION: HACE UNA SECUENCIA COLORES EN EL LED D1 Y ELIMINA UN COLOR DE LA SECUENCIA DEPENDIENDO DE LAS PULSACIONES A SWITCHS CONECTADOS AL PUERTO A
 */

#include <stdint.h>
#include "tm4c123gh6pm.h"

void config_portA(void) {
	// activamos la señal de reloj del puerto A
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;
	// esperamos a que realmente se active
	while( (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R0)==0) {}
	// SW1 está conectado al pin PA4, SW2 al PA3, SW3 al PA2 y SW4 al PA5.
	GPIO_PORTA_DIR_R &= ~(0x3C); // PA5, PA4, PA3 y PA2 como entrada
	GPIO_PORTA_AFSEL_R &= ~(0x3C); // No utilizar funciones alternas
	GPIO_PORTA_PUR_R |= 0x3C; // Activar resistencias de pull-up en pines PA5, PA4, PA3 y PA2
	GPIO_PORTA_DEN_R |= 0x3C; // Habilitar pines PA5, PA4, PA3 y PA2 en funcion digital
}

void config_leds(void) {
	// activamos la señal de reloj del puerto F
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
	// esperamos a que realmente se active
	while( (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)==0) {}
	GPIO_PORTF_DIR_R |= 0x0E; // Configura el bit 2 del puerto F como salida
	GPIO_PORTF_DR8R_R |=0x0E; // se activa el driver de 8 mA en el pin 2.
	GPIO_PORTF_DEN_R |=0x0E; // se activa el pin 2, como salida digital.
	GPIO_PORTF_DATA_R &= ~(0X0E); // apagamos el led en PF2 (azul)
}

void main(void)
{
	uint32_t cuenta = 8;
	uint32_t eliminado = 9;
	uint32_t cent1=0;
	uint32_t cent2=0;
	uint32_t cent3=0;
	uint32_t temp = 0;
	uint32_t number_temp = 0;
	uint32_t n;
	config_portA();
	config_leds();

	while(1)
	{

		temp=GPIO_PORTA_DATA_R;

		if ((temp&0x10)==0x10)//pulsador 1 apretado
		{
			if(cent1==0){number_temp = number_temp + 4; cent1=1;}else{number_temp = number_temp - 4; cent1=0;}
			do {
				temp=GPIO_PORTA_DATA_R; // espera que sea liberado (soltado)
			  } while((temp&0x10)==0x10);
		}

		if ((temp&0x08)==0x08)//pulsador 2 apretado
		{
			if(cent2==0){number_temp = number_temp + 2; cent2=1;}else{number_temp = number_temp - 2; cent2=0;}
			do {
				temp=GPIO_PORTA_DATA_R; // espera que sea liberado (soltado)
			   } while((temp&0x08)==0x08);
		}

		if ((temp&0x04)==0x04)//pulsador 3 apretado
		{
			if(cent3==0){number_temp = number_temp + 1; cent3=1;}else{number_temp = number_temp - 1; cent3=0;}
			do {
				temp=GPIO_PORTA_DATA_R; // espera que sea liberado (soltado)
			   } while((temp&0x04)==0x04);
		}

		if ((temp&0x20)==0x20)//pulsador 4 apretado
		{
			eliminado =number_temp;
			number_temp=0;
			cent1=0;
			cent2=0;
			cent3=0;
			do {
				temp=GPIO_PORTA_DATA_R; // espera que sea liberado (soltado)
			   } while((temp&0x08)==0x08);
		}

		if (cuenta ==eliminado){}
		else
		{GPIO_PORTF_DATA_R &= ~(0X0E);
		GPIO_PORTF_DATA_R |=cuenta*2;
		for(n = 0; n < 100000; n++);}

		if(cuenta==0)
		{cuenta = 7;}
		else
		{cuenta--;}
	}
}
