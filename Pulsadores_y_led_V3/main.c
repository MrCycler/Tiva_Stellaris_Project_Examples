/*
 PROGRAMA: PULSADORES Y LED ( VERSION 2)
 Autor: RKZ
 DESCRIPCION: HACE UNA SECUENCIA DIFERENTE DE COLORES EN EL LED D1 DEPENDIENDO SI SE PRESIONA SW1 O SW2
 AL SALIR DE UNA SECUENCIA ALMACENA UN COLOR Y SI SE ALMACENA 1 PARA CADA UNA DE ESTAS MUESTRA UNA SECUENCIA
 ADICIONAL DONDE SE MUESTRAN AMBOS COLORES ALMACENADOS Y SU MEZCLA
 */
#include <stdint.h>
#include "tm4c123gh6pm.h"

void config_sws(void){
	// activamos la senal de reloj del puerto F
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
	// esperamos a que realmente se active
	while( (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)==0) { }
	// SW1 esta conectado al pin PF4 y SW2 al PF0 (Desbloqueamos el SW2)
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R = 0x1F;
	//Configuraciones de puertos
	GPIO_PORTF_DIR_R &= ~(0x11); // PF4 y PF0 pin de entrada
	GPIO_PORTF_AFSEL_R &= ~(0x11); // no usamos funcion alternativa
	GPIO_PORTF_PUR_R |= 0x11; // activamos resistencia de pull-up en pin PF4
	GPIO_PORTF_DEN_R |= 0x11; // PF4 pin digital
}

void config_leds(void){
	// activamos la senal de reloj del puerto F
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
	// esperamos a que realmente se active
	while( (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)==0) { }
	GPIO_PORTF_DIR_R |= 0x0E; // Configura el bit 1 del puerto F como salida
	GPIO_PORTF_DR8R_R |=0x0E; // se activa el driver de 8 mA en el pin 1.
	GPIO_PORTF_DEN_R |=0x0E; // se activa el pin 1, como salida digital.
	GPIO_PORTF_DATA_R &= ~(0x0E); // apagamos el led
}


void main(void) {
	uint32_t llave1=1;//Valor bandera 1 que impide la permanencia en un bucle
	uint32_t llave2=1;//Valor bandera 2 que impide la permanencia en un bucle
	int n=0; //Variable auxiliar
	uint8_t N1=0;
	uint8_t M1=0;
	uint8_t O1=0;
	uint8_t N2=0;
	uint8_t M2=0;
	uint8_t O2=0;
	config_sws(); // Configura los switches
	config_leds(); //Configura los leds
	while(1){
				GPIO_PORTF_DATA_R  &= ~(0x0E);
				while(llave1==0)
				{

					GPIO_PORTF_DATA_R  &= ~(0x0E) ;//Cambia el color y espera un tiempo
					GPIO_PORTF_DATA_R |=0x02 ;
					n=0;
					while((n<800000)&&(llave1==0))
					{
						n++;
						while((GPIO_PORTF_DATA_R& 0x01) == 0)//Si se presiona el sw1 sale del bucle
						{	llave1=1;
							GPIO_PORTF_DATA_R  &= ~(0x0E); //Apaga el led
							N1=0x02; //Almacena
							M1=0x04;
							O1=0x08;
						}
					}
					///////////////////////////////////////////////////////
					GPIO_PORTF_DATA_R  &= ~(0x0E) ;//Cambia el color y espera un tiempo
					GPIO_PORTF_DATA_R |=0x04 ;
					n=0;
					while((n<800000)&&(llave1==0))
					{
						n++;
						while((GPIO_PORTF_DATA_R& 0x01) == 0)//Si se presiona el sw1 sale del bucle
						{	llave1=1;
							GPIO_PORTF_DATA_R  &= ~(0x0E); //Apaga el led
							N1=0x04; //Almacena
							M1=0x02;
							O1=0x08;
						}
					}
					/////////////////////////////////////////////////////
					GPIO_PORTF_DATA_R  &= ~(0x0E) ;//Cambia el color y espera un tiempo
					GPIO_PORTF_DATA_R |=0x08 ;
					n=0;
					while((n<800000)&&(llave1==0))
					{
						n++;
						while((GPIO_PORTF_DATA_R& 0x01) == 0)//Si se presiona el sw1 sale del bucle
						{	llave1=1;
							GPIO_PORTF_DATA_R  &= ~(0x0E); //Apaga el led
							N1=0x08; //Almacena
							M1=0x02;
							O1=0x04;
						}
					}


				}
				while(llave2==0)
				{

					GPIO_PORTF_DATA_R  &= ~(0x0E) ;//Cambia el color y espera un tiempo
					GPIO_PORTF_DATA_R |=0x08 ;
					n=0;
					while((n<800000)&&(llave2==0))
					{
						n++;
						while((GPIO_PORTF_DATA_R& 0x10) == 0)//Si se presiona el sw1 sale del bucle
						{	llave2=1;
							GPIO_PORTF_DATA_R  &= ~(0x0E); //Apaga el led
							N2 = 0x08; //Almacena
							M2 = 0x02;
							O2 = 0x04;
						}
					}
					/////////////////////////////
					GPIO_PORTF_DATA_R  &= ~(0x0E) ;//Cambia el color y espera un tiempo
					GPIO_PORTF_DATA_R |=0x04 ;
					n=0;
					while((n<800000)&&(llave2==0))
					{
						n++;
						while((GPIO_PORTF_DATA_R& 0x10) == 0)//Si se presiona el sw1 sale del bucle
						{	llave2=1;
							GPIO_PORTF_DATA_R  &= ~(0x0E); //Apaga el led
							N2 = 0x04; //Almacena
							M2 = 0x02;
							O2 = 0x08;
						}
					}
					//////////////////////////////////////
					GPIO_PORTF_DATA_R  &= ~(0x0E) ;//Cambia el color y espera un tiempo
					GPIO_PORTF_DATA_R |=0x02 ;
					n=0;
					while((n<800000)&&(llave2==0))
					{
						n++;
						while((GPIO_PORTF_DATA_R& 0x10) == 0)//Si se presiona el sw1 sale del bucle
						{	llave2=1;
							GPIO_PORTF_DATA_R  &= ~(0x0E); //Apaga el led
							N2 = 0x02; //Almacena
							M2 = 0x04;
							O2 = 0x08;
						}
					}
				}

				if((N2 !=0)&&(N1!=0))
				{
					if (N2==N1){
						GPIO_PORTF_DATA_R  &= ~(0x0E) ;//Cambia el color y espera un tiempo
						for(n=0;n<200000;n++);
						GPIO_PORTF_DATA_R |=N1 ;
						for(n=0;n<200000;n++);
					}

					else
					{
						GPIO_PORTF_DATA_R  &= ~(0x0E) ;//Cambia el color y espera un tiempo
						GPIO_PORTF_DATA_R |=(O1|M1) ;
						for(n=0;n<800000;n++);
						GPIO_PORTF_DATA_R  &= ~(0x0E) ;//Cambia el color y espera un tiempo
						GPIO_PORTF_DATA_R |=(O2|M2) ;
						for(n=0;n<800000;n++);
					}

				}
				while((GPIO_PORTF_DATA_R& 0x10) == 0)
				{llave1=0;
				GPIO_PORTF_DATA_R  &= ~(0x0E) ;
				}//se aprieta sw1 cambia la llave 1

				while((GPIO_PORTF_DATA_R& 0x01) == 0)
				{llave2=0;
				GPIO_PORTF_DATA_R  &= ~(0x0E) ;
				} //se aprieta sw2 cambia la llave 2
	}
}
