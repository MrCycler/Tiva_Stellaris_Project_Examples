#include <stdint.h>
#include "tm4c123gh6pm.h"

 void Setup_PWM0_3(void);//Port:PC4
 void Start_PWM0_3(uint32_t Freq, uint32_t DC1A);//Freq en Hz,DC en 1-->0.01%
  
 void Setup_PWM0_0(void);//Port: PB6,PB7
 void Start_PWM0_0(uint32_t Freq, uint32_t DC2A,uint32_t DC2B);//Freq en Hz,DC's en 1-->0.01%
 void Setup_PWM0_1(void);//Port: PB4
 void Start_PWM0_1(uint32_t Freq, uint32_t DC3A);//Freq en Hz,DC's en 1-->0.01%

/*Función de configuración de salida PWM0 Salida:PC4*/
 void Setup_PWM0_3(void){
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
	/*PWM0_3_GENA_R &= ~ ((3<<10) | (3<<0));
	PWM0_3_GENA_R |= (2<<2)|(3<<6);//PC4 es 1*/
	PWM0_3_GENA_R &= ~(0xFFF);
	PWM0_3_GENA_R = (PWM_0_GENA_ACTCMPAD_ONE|PWM_0_GENA_ACTLOAD_ZERO);
	//PWM0_3_GENB_R = (PWM_0_GENB_ACTCMPBD_ONE|PWM_0_GENB_ACTLOAD_ZERO);
}

/*Función para generar una señal PWM0 en PC4 (PWM0)*/
void Start_PWM0_3( uint32_t Freq, uint32_t DC1A){
 	uint32_t valor_freq;
	uint32_t valor_DC1;
	PWM0_ENABLE_R &= ~0x40; // Deshabilita PWM0
	PWM0_3_CTL_R &= ~1; //Desactiva generador 3 Modulo 0
	valor_freq=4000000/(Freq);
	valor_DC1= (DC1A*(valor_freq +1)/10000)-1; //Porcentaje de la carga DC%
	PWM0_3_LOAD_R &= ~0xFFFF;
	PWM0_3_LOAD_R |= valor_freq;//Carga el valor para la freq en PWM_LOAD
	PWM0_3_CMPA_R &= ~0xFFFF;
	PWM0_3_CMPA_R |= valor_DC1; // carga el valor para el DC en CMPA
	PWM0_3_CTL_R |=1; //Activa generador 3 Modulo 0
	PWM0_ENABLE_R |= (1<<6); //habilita PWM0
}

/*Función de configuración de salida PWM0 Salida: PB6, PB7*/
 void Setup_PWM0_0(void){
	uint32_t temp;
	//1- Se activa la senal de reloj para el módulo PWM y el puerto B.
	SYSCTL_RCGC0_R |= SYSCTL_RCGC0_PWM0; //SYSCTL_RCGC0_PWM0 = 0x00100000
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; //SYSCTL_RCGC2_GPIOB = 0x2
	//2- Retardo
	temp = SYSCTL_RCGC0_R;
	temp = SYSCTL_RCGC0_R;
	temp = SYSCTL_RCGC0_R;
	temp = SYSCTL_RCGC0_R;
	temp = SYSCTL_RCGC0_R;
	temp = SYSCTL_RCGC0_R;
	temp = SYSCTL_RCGC0_R;
	temp = SYSCTL_RCGC0_R;
	//3- Configurar el divisor PWM: USEPWMDIV = 1.
	//Dividir 16 MHz/64 = Frecuencia de reloj del módulo PWM = 250 KHz
	//ADVERTENCIA!! Modificar esta instrucción con precaución. El registro SYSCTL_RCC_R
	//también contiene bits de control para el reloj principal del microcontrolador. Una acción //incorrecta podría dejar inutilizable el microcontrolador
	SYSCTL_RCC_R|= SYSCTL_RCC_USEPWMDIV;
	temp= SYSCTL_RCC_R;
	temp= SYSCTL_RCC_R; //Retardos de estabilización
	temp= SYSCTL_RCC_R;
	temp= SYSCTL_RCC_R;
	//4- Deshabilitar generador PWM0 previo a la configuración
	PWM0_0_CTL_R &= ~(PWM_0_CTL_ENABLE); //PWM_0_CTL_ENABLE = 0x1
	//5- Configurar el generador PWM0 para modo de cuenta descendente
	PWM0_0_CTL_R &= ~(0x3F); //Estado por defecto, campo de bits (5:0) en '0' lógico
	PWM0_0_GENA_R &= ~(0xFFF);
	PWM0_0_GENA_R = (PWM_0_GENA_ACTCMPAD_ONE|PWM_0_GENA_ACTLOAD_ZERO);
	PWM0_0_GENB_R &= ~(0xFFF);
	PWM0_0_GENB_R = (PWM_0_GENB_ACTCMPBD_ONE|PWM_0_GENB_ACTLOAD_ZERO);
	//9- Se configura los pines de salida para el puerto B (PB6 y PB7)
	//Se activa la función alterna para trabajar con PWM
	GPIO_PORTB_AFSEL_R |= 0xC0;
	//Configura el registro PCTL para elegir la función
	GPIO_PORTB_PCTL_R &= ~0xFF000000;
	GPIO_PORTB_PCTL_R |= 0x44000000; // Configura PB6 y PB7 como PWM0
	//Deshabilita funciones analógicas sobre PB6 y PB7
	GPIO_PORTB_AMSEL_R &= ~0xC0;
	//Habilita la función digital sobre el pin PB6 y PB7
	GPIO_PORTB_DEN_R |= 0xC0;
}

/*Función para generar una señal PWM0 en PB6 y PB7 (PWM0)*/
void Start_PWM0_0( uint32_t Freq, uint32_t DC2A, uint32_t DC2B){
	uint32_t valor_freq;
	uint32_t valor_DC1;
	uint32_t valor_DC2;
	PWM0_ENABLE_R &= ~(PWM_ENABLE_PWM0EN|PWM_ENABLE_PWM1EN);
	PWM0_0_CTL_R &=~PWM_0_CTL_ENABLE;
	valor_freq=4000000/(Freq);
	valor_DC1= (DC2A*(valor_freq +1)/10000)-1; //Porcentaje de la carga DC1%
	valor_DC2= (DC2B*(valor_freq +1)/10000)-1; //Porcentaje de la carga DC2%
	PWM0_0_LOAD_R &= ~(PWM_0_LOAD_M);
	PWM0_0_LOAD_R |= valor_freq;
	PWM0_0_CMPA_R &= ~(PWM_0_CMPA_M);
	PWM0_0_CMPA_R |= valor_DC1;
	PWM0_0_CMPB_R &= ~(PWM_0_CMPB_M); 
	PWM0_0_CMPB_R |= valor_DC2;
	//10- Se habilita el generador PWM0, inicia los contadores
	PWM0_0_CTL_R |= PWM_0_CTL_ENABLE; //PWM_0_CTL_ENABLE = 0x1
	//11- Se habilita la salida de la señal PWM0_0 en PB6 y PWM0_1 en PB7
	PWM0_ENABLE_R |= (PWM_ENABLE_PWM0EN|PWM_ENABLE_PWM1EN);
}

/*Función de configuración de salida PWM0 Salida: PB4*/
void Setup_PWM0_1(void){
	uint32_t temp;
	SYSCTL_RCGC0_R |= 0X100000; //Se habilita el reloj para el PWM0
	SYSCTL_RCGC2_R |=SYSCTL_RCGC2_GPIOB; //Habilitamos el puerto C para seleccionar PC4 como PWM
	temp=SYSCTL_RCGC2_R;
	GPIO_PORTB_DIR_R |= (1<<4);
	GPIO_PORTB_DR8R_R |=(1<<4);
	GPIO_PORTB_DEN_R |= (1<<4);
	GPIO_PORTB_AFSEL_R |= (1<<4); // Funcion Alterna para PB4, no GPIO
	GPIO_PORTB_PCTL_R=((GPIO_PORTB_PCTL_R & 0xFFF0FFFF)|0x00040000); // Seleccionamos
	//funcion 4 (PWM) de PC4
	SYSCTL_RCC_R = (SYSCTL_RCC_R & ~0x1E0000)| (9<<17); // Pre: 64
	PWM0_1_CTL_R= ((PWM0_3_CTL_R &~ ((3<<6)|(1<<4)|(1<<3)|(1<<1)|(1<<0)))|(1<<2)); //cuenta
	//descendente y sincronizacion local
	/*PWM0_3_GENA_R &= ~ ((3<<10) | (3<<0));
	PWM0_3_GENA_R |= (2<<2)|(3<<6);//PC4 es 1*/
	PWM0_1_GENA_R &= ~(0xFFF);
	PWM0_1_GENA_R = (PWM_0_GENA_ACTCMPAD_ONE|PWM_0_GENA_ACTLOAD_ZERO);
	//PWM0_3_GENB_R = (PWM_0_GENB_ACTCMPBD_ONE|PWM_0_GENB_ACTLOAD_ZERO);
}

/*Función para generar una señal PWM0 en PB6 y PB7 (PWM0)*/
void Start_PWM0_1( uint32_t Freq, uint32_t DC3A){
	uint32_t valor_freq;
	uint32_t valor_DC1;
	PWM0_ENABLE_R &= ~0x40; // Deshabilita PWM0
	PWM0_1_CTL_R &= ~1; //Desactiva generador 3 Modulo 0
	valor_freq=4000000/(Freq);
	valor_DC1= (DC3A*(valor_freq +1)/10000)-1; //Porcentaje de la carga DC%
	PWM0_1_LOAD_R &= ~0xFFFF;
	PWM0_1_LOAD_R |= valor_freq;//Carga el valor para la freq en PWM_LOAD
	PWM0_1_CMPA_R &= ~0xFFFF;
	PWM0_1_CMPA_R |= valor_DC1; // carga el valor para el DC en CMPA
	PWM0_1_CTL_R |=1; //Activa generador 3 Modulo 0
	PWM0_ENABLE_R |= (1<<6); //habilita PWM0
}
