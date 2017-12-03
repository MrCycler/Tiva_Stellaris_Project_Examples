/*****************************************************************************
* Nombre: Tarea C2
* M�dulos: M�dulo Tiva Launchpad EK-TM4C123GXL y M�dulo E/S Tiva-PUCP

******************************************************************************/
	#include <stdint.h>
	#include "tm4c123gh6pm.h"
//**************************************************************************
// funcion config_led_rojo para configurar el pin PF1 como salida
		void config_led(void) {
			// activamos la se�al de reloj del puerto F
			SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
			// esperamos a que realmente se active
			while( (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)==0) {}
				GPIO_PORTF_DIR_R |= 0x0E; // Configura el bit 1 del puerto F como salida
				GPIO_PORTF_DR8R_R |=0x0E; // se activa el driver de 8 mA en el pin 1.
				GPIO_PORTF_DEN_R |=0x0E; // se activa el pin 1, como salida digital.
				GPIO_PORTF_DATA_R &= ~(0X0E); // apagamos el led en PF1 (rojo)
				} // fin config_led_rojo

***********************************************************************************************
//PROGRAMA PRINCIPAL
void main (void){

	uint_32 color1, color2, color3, color4, color5, colortemp, number;
	uint_32 cent1,cent2,cent3,posi,nega;
	uint_32 num_temp;
	config_led();
	

	while(1){
		//guardado
		number=0;
		color1=0;
		color2=0;
		color3=0;
		color4=0;
		color5=0;
		centM=0;
		posi=0;
		nega=0;

		while((number<5)&&(centM==0))
		{
			colortemp=0;
			cent1=0;
			cent2=0;
			cent3=0;

			//definir presionado1,2,3
			if(presionado1()==1){
				if(cent1==0)
				{colortemp=colortemp+2;cent1=1;}
				else
				{colortemp=colortemp-2;cent1=0;}
			}
			if(presionado2()==1){
					if(cent2==0)
					{colortemp=colortemp+4;cent2=1;}
					else
					{colortemp=colortemp-4;cent2=0;}
				}
			if(presionado3()==1){
					if(cent3==0)
					{colortemp=colortemp+8;cent3=1;}
					else
					{colortemp=colortemp-8;cent3=0;}
				}
			if((presionado4()==1){
				switch(number){
				case 1: color1=colortemp;
				case 2: color2=colortemp;
				case 3: color3=colortemp;
				case 4: color4=colortemp;
				case 5: color5=colortemp;
				}
				number++;
				}
			if((presionadotiva1()==1)&&(number>2)){
				centM=1;posi=1; nega=0;
			}
			if((presionadotiva2()==1)&&(number>2)){
				centM=1;posi=0; nega=1;
			}
		}
		centM=0;
		//mostrar
		while(centM==0)
		{
				num_temp=1;
				while((posi==1)&&(centM==0)){
				switch(num_temp){
					case 1: GPIO_PORTF_DATA_R &= ~(0X0E); GPIO_PORTF_DATA_R |=color1 ;for(n = 0; n < 900000; n++);
					case 2: GPIO_PORTF_DATA_R &= ~(0X0E); GPIO_PORTF_DATA_R |=color2 ;for(n = 0; n < 900000; n++);
					case 3: GPIO_PORTF_DATA_R &= ~(0X0E); GPIO_PORTF_DATA_R |=color3 ;for(n = 0; n < 900000; n++);
					case 4: GPIO_PORTF_DATA_R &= ~(0X0E); GPIO_PORTF_DATA_R |=color4 ;for(n = 0; n < 900000; n++);
					case 5: GPIO_PORTF_DATA_R &= ~(0X0E); GPIO_PORTF_DATA_R |=color5 ;for(n = 0; n < 900000; n++);
					}
				num_temp++; if(num_temp>number){num_temp=1;}

				if(presionadotiva2()==1){
					posi=0; nega=1;
				}
				if(presionado4()==1){
					centM=1;
				}

				}

				num_temp=number;
				while((nega==1)&&(centM==0)){
				switch(num_temp){
					case 1: GPIO_PORTF_DATA_R &= ~(0X0E); GPIO_PORTF_DATA_R |=color1 ;for(n = 0; n < 900000; n++);
					case 2: GPIO_PORTF_DATA_R &= ~(0X0E); GPIO_PORTF_DATA_R |=color2 ;for(n = 0; n < 900000; n++);
					case 3: GPIO_PORTF_DATA_R &= ~(0X0E); GPIO_PORTF_DATA_R |=color3 ;for(n = 0; n < 900000; n++);
					case 4: GPIO_PORTF_DATA_R &= ~(0X0E); GPIO_PORTF_DATA_R |=color4 ;for(n = 0; n < 900000; n++);
					case 5: GPIO_PORTF_DATA_R &= ~(0X0E); GPIO_PORTF_DATA_R |=color5 ;for(n = 0; n < 900000; n++);
					}
				num_temp++; if(num_temp>number){num_temp=1;}

				if(presionadotiva1()==1){
					posi=1; nega=0;
				}
				if(presionado4()==1){
					centM=1;
				}

				}
		}
	}
}
