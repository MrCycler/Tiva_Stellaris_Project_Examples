/*
 * main.c
 */
/////////////////////////////////////////////////BOMBA PARTE A)

#include <stdint.h>
#include "tm4c123gh6pm.h"

/*Funcion de configuración de UART1*/
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

/*Función de configuración de salida PWM
* Salida: PC4
* */
void config_PWM(void){
	uint32_t temp;
	SYSCTL_RCGC0_R |= 0X100000; //Se habilita el reloj para el PWM0
	SYSCTL_RCGC2_R |=SYSCTL_RCGC2_GPIOC; //Habilitamos el puerto C para seleccionar PC4 como PWM
	temp=SYSCTL_RCGC2_R;
	GPIO_PORTC_DIR_R |= (1<<4);
	GPIO_PORTC_DR8R_R |=(1<<4);
	GPIO_PORTC_DEN_R |= (1<<4);
	GPIO_PORTC_AFSEL_R |= (1<<4); // Funcion Alterna para PC4, no GPIO
	GPIO_PORTC_PCTL_R=((GPIO_PORTC_PCTL_R & 0xFFF0FFFF)|0x00040000); // Seleccionamos
	//funcion 4 (PWM) de PC4
	SYSCTL_RCC_R = (SYSCTL_RCC_R & ~0x1E0000)| (9<<17); // Pre: 64
	PWM0_3_CTL_R= ((PWM0_3_CTL_R &~ ((3<<6)|(1<<4)|(1<<3)|(1<<1)|(1<<0)))|(1<<2)); //cuenta
	//descendente y sincronizacion local
	PWM0_3_GENA_R &= ~ ((3<<10) | (3<<0));
	PWM0_3_GENA_R |= (2<<2)|(3<<6);//PC4 es 1
}

/*Función para generar una señal PWM*/
void PWM_OUT( uint32_t Freq, uint32_t DC)
{ 	uint32_t valor_freq;
	uint32_t valor_DC;
	PWM0_ENABLE_R &= ~0x40; // Deshabilita PWM0
	PWM0_3_CTL_R &= ~1; //Desactiva generador 3 Modulo 0
	valor_freq=4000000/(Freq);
	valor_DC= (DC*(valor_freq +1)/10000)-1; //Porcentaje de la carga DC%
	PWM0_3_LOAD_R &= ~0xFFFF;
	PWM0_3_LOAD_R |= valor_freq;//Carga el valor para la freq en PWM_LOAD
	PWM0_3_CMPA_R &= ~0xFFFF;
	PWM0_3_CMPA_R |= valor_DC; // carga el valor para el DC en CMPA
	PWM0_3_CTL_R |=1; //Activa generador 3 Modulo 0
	PWM0_ENABLE_R |= (1<<6); //habilita PWM0
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
void txmens_uart0(uint8_t mens[]){
	uint8_t letra;
	uint8_t i=0;
	letra= mens[i++];
	while (letra != '\0'){ //Se envían todos los caracteres hasta el fin de cadena
		txcar_uart1(letra);
		letra= mens[i++];
		}
}
int rxmens_uart0(uint8_t arreglo[10]){
	uint8_t letra;
	uint8_t i=0;
	int valor=0;
	letra = rxcar_uart1();
	arreglo[i]=letra;
	while (letra != 13){ //Se leen todos los caracteres hasta el fin de cadena
		valor=letra-48+valor*10;
		letra=rxcar_uart1();
		i++;
		arreglo[i]=letra;
	}
	return valor;
}
void main(void){
	/*Configurar puertos*/
	config_PWM(); //SALIDA PC4
	config_UART1();
	/*Inicializar valores*/
	uint8_t Entrada[] = "Introduzca el número de suministro a alimentar:\n\r";
	int valor;
	uint32_t Freq=50;//Frecuencia 50 Hz - 20 ms
	uint8_t arreglo[10];
	// Se genera la señal PWM con Frecuencia 5KHz, DC=50%
	PWM_OUT (Freq,0);

	while(1){
		txmens_uart0(Entrada);
		valor=rxmens_uart0(arreglo);
		txmens_uart0("\n\r");
		switch(valor)
		{
			case 1:
			PWM_OUT(Freq,250);
			txmens_uart0("Alimentando el suministro 1 \n\r");
			break;
			case 2:
			PWM_OUT(Freq,500);
			txmens_uart0("Alimentando el suministro 2 \n\r");
			break;
			case 3:
			PWM_OUT(Freq,750);
			txmens_uart0("Alimentando el suministro 3 \n\r");
			break;
			case 4:
			PWM_OUT(Freq,1000);
			txmens_uart0("Alimentando el suministro 4 \n\r");
			break;
			case 5:
			PWM_OUT(Freq,1250);
			txmens_uart0("Alimentando el suministro 5 \n\r");
			break;
			default:
			txmens_uart0("Comando inválido \n\r");
		}

		}
}

