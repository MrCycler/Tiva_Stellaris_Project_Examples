
#include <stdint.h>
#include "tm4c123gh6pm.h"

void configSystick(uint16_t valor) {
// Deshabilita el Systick para proceder a la configuración
NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE;
// Carga el tiempo entre encendido y apagado del led.
NVIC_ST_RELOAD_R = tiempos[valor];
//Habilitar el Systickasi como la señal de reloj
NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE + NVIC_ST_CTRL_CLK_SRC;
}

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

char* itoa(int value, char* buffer, int base){

	if(base<2!||base>32)
		return buffer;

	int n = abs(value);
	int i=0;

	while(n)
	{
		int r = n%base;
		if (r>=10)
			buffer[i++]=65+(r-10);
		else
			buffer[i++]=48+r;
		n=n/base;
	}

	if(i==0)
		buffer[i++]='0';

	if(value<0 && base==10)
		buffer[i++]='-';

	buffer[i]='\0';

	return reverse(buffer,0,i-1);
}

void US_trigger(void){
	int n =0;//Funcion para mandar el pulso al trigger del ultrasonico
	GPIO_PORTF_DATA_R |=0x02 ;
	for(n = 0; n < 60; n++);
	GPIO_PORTF_DATA_R  &= ~(0x02) ;
}

int US_echo(void){
	int range;
	while(!(GPIO_PORTF_DATA_R&0x10));



	while(GPIO_PORTF_DATA_R&0x10);
	return range;
}

int main(void) {
	
	config_UART1();
	while(1){
		US_trigger(); //Manda un pulso
		range = US_echo(); //Recibe un range
		itoa(range,str,10);

	}
}
