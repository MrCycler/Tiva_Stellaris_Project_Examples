/*
 * main.c
 */
#include <stdint.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"



#define I2C_MCS_ACK             0x00000008  // Data Acknowledge Enable
#define I2C_MCS_DATACK          0x00000008  // Acknowledge Data
#define I2C_MCS_ADRACK          0x00000004  // Acknowledge Address
#define I2C_MCS_STOP            0x00000004  // Generate STOP
#define I2C_MCS_START           0x00000002  // Generate START
#define I2C_MCS_ERROR           0x00000002  // Error
#define I2C_MCS_RUN             0x00000001  // I2C Master Enable
#define I2C_MCS_BUSY            0x00000001  // I2C Busy
#define I2C_MCR_MFE             0x00000010  // I2C Master Function Enable
#define MAXRETRIES              5           // number of receive attempts before giving up


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

void I2C_Init(void){
  SYSCTL_RCGCI2C_R |= 0x0001;           // activate I2C0
  SYSCTL_RCGCGPIO_R |= 0x0002;          // activate port B
  while((SYSCTL_PRGPIO_R&0x0002) == 0){};// ready?

  GPIO_PORTB_AFSEL_R |= 0x0C;           // 3) enable alt funct on PB2,3
  GPIO_PORTB_ODR_R |= 0x08;             // 4) enable open drain on PB3 only
  GPIO_PORTB_DEN_R |= 0x0C;             // 5) enable digital I/O on PB2,3
                                        // 6) configure PB2,3 as I2C
  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFFF00FF)+0x00003300;
  GPIO_PORTB_AMSEL_R &= ~0x0C;          // 7) disable analog functionality on PB2,3
  I2C0_MCR_R = I2C_MCR_MFE;      // 9) master function enable
  I2C0_MTPR_R = 24;              // 8) configure for 100 kbps clock
  // 20*(TPR+1)*20ns = 10us, with TPR=24
}


void txcar_uart1(uint32_t car){
	//Espera disponibilidad para transmitir
	while(UART0_FR_R & UART_FR_TXFF);
	// Se escribe el dato para su transmisión
	UART0_DR_R = car;
}

uint32_t I2C_Send1(int8_t slave, uint8_t data1){
	while(I2C0_MCS_R & I2C_MCS_BUSY){};// wait for I2C ready
	I2C0_MSA_R = (slave<<1)&0xFE; // MSA[7:1] is slave address
	I2C0_MSA_R &= ~0x01; // MSA[0] is 0 for send
	I2C0_MDR_R = data1&0xFF; // prepare first byte
	I2C0_MCS_R = (0
				// & ~I2C_MCS_ACK // no data ack (no data on send)
				//| I2C_MCS_STOP // generate stop
				| I2C_MCS_START // generate start/restart
				| I2C_MCS_RUN); // master enable
	while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
	// return error bits
	return (I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
}//end I2C_SEND

uint16_t I2C_Recv2(int8_t slave){
	uint8_t data1,data2;
	int retryCounter = 1;
	do{
		while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
		I2C0_MSA_R = (slave<<1)&0xFE; // MSA[7:1] is slave address
		I2C0_MSA_R |= 0x01; // MSA[0] is 1 for receive
		I2C0_MCS_R = (0
					| I2C_MCS_ACK // positive data ack
					// & ~I2C_MCS_STOP // no stop
					| I2C_MCS_START // generate start/restart
					| I2C_MCS_RUN); // master enable
		while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
		data1 = (I2C0_MDR_R&0xFF); // MSB data sent first
		I2C0_MCS_R = (0
					// & ~I2C_MCS_ACK // negative data ack (last byte)
					| I2C_MCS_STOP // generate stop
					// & ~I2C_MCS_START // no start/restart
					| I2C_MCS_RUN); // master enable
		while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
		data2 = (I2C0_MDR_R&0xFF); // LSB data sent last
		retryCounter = retryCounter + 1; // increment retry counter
	} // repeat if error
	while(((I2C0_MCS_R&(I2C_MCS_ADRACK|I2C_MCS_ERROR)) != 0) && (retryCounter <= MAXRETRIES));
	return (data1<<8)+data2; // usually returns 0xFFFF on error
}//end I2C_RECV2

uint32_t I2C_Send2(int8_t slave, uint8_t data1, uint8_t data2){
	while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
  I2C0_MSA_R = (slave<<1)&0xFE;    // MSA[7:1] is slave address
  I2C0_MSA_R &= ~0x01;             // MSA[0] is 0 for send
  I2C0_MDR_R = data1&0xFF;         // prepare first byte
  I2C0_MCS_R = (0
                     //  & ~I2C_MCS_ACK     // no data ack (no data on send)
                    //   & ~I2C_MCS_STOP    // no stop
                       | I2C_MCS_START    // generate start/restart
                       | I2C_MCS_RUN);    // master enable
 // while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
                                          // check error bits
  if((I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR)) != 0){
    I2C0_MCS_R = (0                // send stop if nonzero
                     //  & ~I2C_MCS_ACK     // no data ack (no data on send)
                       | I2C_MCS_STOP     // stop
                     //  & ~I2C_MCS_START   // no start/restart
                     //  & ~I2C_MCS_RUN    // master disable
                        );
                                          // return error bits if nonzero
    return (I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
  }
  I2C0_MDR_R = data2&0xFF;         // prepare second byte
  I2C0_MCS_R = (0
                      // & ~I2C_MCS_ACK     // no data ack (no data on send)
                       | I2C_MCS_STOP     // generate stop
                      // & ~I2C_MCS_START   // no start/restart
                       | I2C_MCS_RUN);    // master enable
  //while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
                                          // return error bits
  return (I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
}

void MPUInit(void)
{	I2C_Send2(0x68,0x19,0x07);
	I2C_Send2(0x68,0x6B,0x01);
	I2C_Send2(0x68,0x1A,0x00);
	I2C_Send2(0x68,0x1B,0x18);
	I2C_Send2(0x68,0x38,0x00);
	}

void main(void) {
	uint16_t Accel;
	uint32_t Error;
	I2C_Init();
	config_UART1();
	MPUInit();//Configura el MPU

	while(1)
	{

		I2C_Send1(0x68,0x3B);//Envia la direccionde la aceleracion)
		Accel=I2C_Recv2(0x68);//Lee la aceleracion en x (HIGH AND LOW) de la direccion 0x68
		txcar_uart1(Accel&0xFF);
		txcar_uart1('\n');
		txcar_uart1('\r');
	}
}
