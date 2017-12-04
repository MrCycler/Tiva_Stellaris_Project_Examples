/*
 * main.c
 */
#include <stdint.h>
#include "tm4c123gh6pm.h"

void config_UART1 (void){
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

void config_sws(void){
	// activamos la señal de reloj del puerto F
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
	// esperamos a que realmente se active
	while( (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)==0) { }
	// SW1 está conectado al pin PF4 y SW2 al PF0
	//////////////////////////////////////////////UNLOCK
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlock GPIO Port F
	GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
	////////////////////////////////////////////////////////////
	GPIO_PORTF_DIR_R &= ~(0x10); // PF4 y PF0 pin de entrada
	GPIO_PORTF_AFSEL_R &= ~(0x10); // no usamos función alternativa
	GPIO_PORTF_PUR_R |= 0x10; // activamos resistencia de pull-up en pin PF4
	GPIO_PORTF_DEN_R |= 0x10; // PF4 pin digital
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



/*Función para la transmisión de un carácter por UART1*/
void txcar_uart1(uint32_t car){
	//Espera disponibilidad para transmitir
	while(UART0_FR_R & UART_FR_TXFF);
	// Se escribe el dato para su transmisión
	UART0_DR_R = car;
}

/*Función para la recepción de un carácter de 8 bits por UART1*/
uint8_t rxcar_uart1(void){
	uint8_t temp;
	while(UART0_FR_R & UART_FR_RXFE);
	// Se lee el dato recibido
	temp = UART0_DR_R;
	return temp;
}

/*Función para el envio de una cadena*/
void txmens(uint8_t mens[]){
	uint8_t letra;
	uint8_t i=0;
	letra= mens[i++];
	while (letra != '\0'){ //Se envían todos los caracteres hasta el fin de cadena
		txcar_uart1(letra);
		letra= mens[i++];
		}
}
int rxnumero(uint8_t arreglo[10]){
	uint8_t letra;
	uint8_t i=0;
	int valor=0;
	letra = rxcar_uart1();
	arreglo[i]=letra;
	while (letra != 13){ //Se leen todos los caracteres hasta el enter
		valor=letra-48+valor*10;
		letra=rxcar_uart1();
		i++;
		arreglo[i]=letra;
	}
	return valor;
}

void envianumero(int numero ){
	uint8_t c;
	uint8_t d;
	uint8_t u;
	c=(int)numero/100;
	if(c!=0){txcar_uart1(c+48);}
	numero=numero%100;
	d=(int)numero/10;
	if(d!=0){txcar_uart1(d+48);}
	if((d==0)&&(c!=0)){txcar_uart1(d+48);}
	numero=numero%10;
	u =(int)numero;
	if(u!=0){txcar_uart1(u+48);}
	if((u==0)&&(c!=0)){txcar_uart1(u+48);}
	if((u==0)&&(d!=0)){txcar_uart1(u+48);}
}

void main(void) {
	config_UART1();
	config_sws();
	config_leds();
	uint8_t posicionesx[30];
	uint8_t posicionesy[30];
	uint8_t numero = 1;
	uint8_t clave = 0;
	uint8_t arreglo[10];
    int valor;
    while(1){
		GPIO_PORTF_DATA_R |=0x04;//LED EN AZUL
		txmens("¡BIENVENIDO! MAQUINA XY DE CORTE EN ESPERA DE COMANDO:\n\r");
		while((GPIO_PORTF_DATA_R& 0x10) == 0x10) //Espera que sea presionado SW1
		while((GPIO_PORTF_DATA_R& 0x10) == 0)//Espera que sea soltado	SW1
		GPIO_PORTF_DATA_R &= ~(0x0E);

		while (clave == 0){
			txmens("INTRODUZCA LA POSICION ");
			envianumero(numero);
			txmens("(X, Y) DEL POLIGONO DE CORTE EN CM:\n\r");
			valor= rxnumero(arreglo);
			while((valor>100)||(valor<0)){
				txmens("COORDENADA X INCORRECTA, VUELVA A INTRODUCIRLA \n\r");
				valor= rxnumero(arreglo);
			}
			posicionesx[numero-1]=valor;
			txmens("SE INTRODUJO LA COORDENADA X =");
			envianumero(valor);
			txmens("CM, FALTA LA COORDENADA Y \n\r");
			valor= rxnumero(arreglo);
			while((valor>120)||(valor<0)){
				txmens("COORDENADA Y INCORRECTA, VUELVA A INTRODUCIRLA \n\r");
				valor= rxnumero(arreglo);
			}
			posicionesy[numero-1]=valor;
			txmens("POSICION 1 (");
			envianumero(posicionesx[numero-1]);
			txmens(",");
			envianumero(posicionesy[numero-1]);
			txmens(") CORRECTAMENTE INGRESADA \n\r");
			numero++;
			if((GPIO_PORTF_DATA_R& 0x01) == 1)
			{if((numero>1)&&(numero<30)){clave=1;}}
			if (numero==30){clave = 1;}
		}

		GPIO_PORTF_DATA_R |=0x08;//LED EN AZUL
	}

}
