#include <stdint.h>
#include "tm4c123gh6pm.h"
#include <time.h>
#include <stdlib.h>
void config_uart2(void){
	unsigned long temp;
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART2;
	temp = SYSCTL_RCGC1_R;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
	temp = SYSCTL_RCGC2_R;
	UART2_CTL_R &= ~UART_CTL_UARTEN;
	UART2_IBRD_R = (UART2_IBRD_R & ~UART_IBRD_DIVINT_M)|104;
	UART2_FBRD_R = (UART2_FBRD_R & ~UART_FBRD_DIVFRAC_M)|10;
	UART2_LCRH_R = ((UART2_LCRH_R & ~0x000000FF)|(UART_LCRH_WLEN_8)|(UART_LCRH_FEN));
	UART2_CTL_R |= UART_CTL_UARTEN;
	GPIO_PORTD_LOCK_R = 0x4c4f434b; //Se elimina la protección en puerto D
	GPIO_PORTD_CR_R |= 0X80;
	GPIO_PORTD_AMSEL_R &=~0xC0;
	GPIO_PORTD_PUR_R &=~0xC0;
	GPIO_PORTD_PDR_R &=~0xC0;
	GPIO_PORTD_DIR_R &=~0x40;
	GPIO_PORTD_DIR_R |= 0x80;
	GPIO_PORTD_DEN_R |= 0xC0;
	GPIO_PORTD_AFSEL_R |= 0xC0;
	GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0x00FFFFFF)|0x11000000;
	////////////////////////
}
void config_leds(void){
	unsigned long temp;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	temp=SYSCTL_RCGC2_R;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
	temp=SYSCTL_RCGC2_R;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC;
	temp=SYSCTL_RCGC2_R;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
	temp=SYSCTL_RCGC2_R;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
	temp=SYSCTL_RCGC2_R;
	// pines como salida
	GPIO_PORTA_DIR_R |= 0x18;
	GPIO_PORTB_DIR_R |= 0x0C;
	GPIO_PORTC_DIR_R |= 0xF0;
	GPIO_PORTE_DIR_R |= 0x01;
	GPIO_PORTF_DIR_R |= 0x1C;

	GPIO_PORTA_AMSEL_R &=~0x18;
	GPIO_PORTB_AMSEL_R &=~0x0C;
	GPIO_PORTC_AMSEL_R &=~0xF0;
	GPIO_PORTE_AMSEL_R &=~0x01;
	GPIO_PORTF_AMSEL_R &=~0x1C;

	GPIO_PORTA_AFSEL_R &=~0x18;
	GPIO_PORTB_AFSEL_R &=~0x0C;
	GPIO_PORTC_AFSEL_R &=~0xF0;
	GPIO_PORTE_AFSEL_R &=~0x01;
	GPIO_PORTF_AFSEL_R &=~0x1C;

	GPIO_PORTA_DEN_R |= 0x18;
	GPIO_PORTB_DEN_R |= 0x0C;
	GPIO_PORTC_DEN_R |= 0xF0;
	GPIO_PORTE_DEN_R |= 0x01;
	GPIO_PORTF_DEN_R |= 0x1C;
}
void turn_on_led(uint8_t led){
	switch(led){
	case 1: GPIO_PORTB_DATA_R |= 0x04; break;
	case 2: GPIO_PORTE_DATA_R |= 0x01; break;
	case 3: GPIO_PORTA_DATA_R |= 0x10; break;
	case 4: GPIO_PORTA_DATA_R |= 0x08; break;
	case 5: GPIO_PORTF_DATA_R |= 0x04; break;
	case 6: GPIO_PORTF_DATA_R |= 0x08; break;
	case 7: GPIO_PORTB_DATA_R |= 0x08; break;
	case 8: GPIO_PORTC_DATA_R |= 0x10; break;
	case 9: GPIO_PORTC_DATA_R |= 0x20; break;
	case 10: GPIO_PORTC_DATA_R |= 0x40; break;
	case 11: GPIO_PORTC_DATA_R |= 0x80; break;
	case 12: GPIO_PORTF_DATA_R |= 0x10; break;
	default: break;
	}
}
void turn_off_led(uint8_t led){
	switch(led){
	case 1: GPIO_PORTB_DATA_R &=~0x04; break;
	case 2: GPIO_PORTE_DATA_R &=~0x01; break;
	case 3: GPIO_PORTA_DATA_R &=~0x10; break;
	case 4: GPIO_PORTA_DATA_R &=~0x08; break;
	case 5: GPIO_PORTF_DATA_R &=~0x04; break;
	case 6: GPIO_PORTF_DATA_R &=~0x08; break;
	case 7: GPIO_PORTB_DATA_R &=~0x08; break;
	case 8: GPIO_PORTC_DATA_R &=~0x10; break;
	case 9: GPIO_PORTC_DATA_R &=~0x20; break;
	case 10: GPIO_PORTC_DATA_R &=~0x40; break;
	case 11: GPIO_PORTC_DATA_R &=~0x80; break;
	case 12: GPIO_PORTF_DATA_R &=~0x10; break;
	default: break;
	}
}
void systick(uint32_t var){
	uint32_t time;
	time=((var*16000)-1);
	NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE;
	NVIC_ST_RELOAD_R = time;
	NVIC_ST_CURRENT_R=0;
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE + NVIC_ST_CTRL_CLK_SRC;
}
uint8_t lee(void){
	uint8_t dato;
	if((UART2_FR_R & UART_FR_RXFE)==0){
		dato= UART2_DR_R & 0XFF;
	}
	else{
		dato=0;
	}
	return dato;
}
void mostrar_leds(uint16_t valor){
	GPIO_PORTA_DATA_R = ((valor&0x300)>>5);
	GPIO_PORTB_DATA_R = ((valor&0x20)>>2);
	GPIO_PORTC_DATA_R = (((valor&0x02)<<6)|((valor&0x04)<<4)|((valor&0x08)<<2)|((valor&0x10)));
	GPIO_PORTE_DATA_R = ((valor&0x400)>>10);
	GPIO_PORTF_DATA_R = (((valor&0x01)<<4)|((valor&0x40)>>3)|((valor&0x80)>>5));
	systick(16);
	while ((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT)==0){}
}
void main(void){
	uint8_t i,n;
	uint8_t letra;
	config_uart2();
	config_leds();
	uint16_t camino[8]={0,0,0,0,0,0,0,0};

	for(n=1;n<13;n++){
		turn_off_led(n);
	}

	i=rand()%8; //genera un alien aleatorio
	camino[i]=1;

	for(n=0;n<8;n++){
	mostrar_leds(camino[n]);
	}

	while(1){
		for(n=0;n<8;n++){
						camino[n]=2*camino[n];
						}
		for(n=0;n<8;n++){
					if(camino[n]>=1024){
					camino[n]=camino[n]-1024;
					}
						}

		i++;//
		i=i%8;
		camino[i]=camino[i]+1;


		for(n=0;n<8;n++){
		mostrar_leds(camino[n]);
		//mostrar_nada();
		}
	}
}
