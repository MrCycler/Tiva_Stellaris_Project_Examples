/*
 MAIN.c
 Autor: Juan M. Mendoza RKZ
 Fecha: Julio 2017
 Descripcion: Programa para usar pantalla LCD 16x2 con las librerias LCD.h y systick.h
 */
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "LCD.h"
#include "systick.h"


int main(void) {
	   LCD_Init(); //Inicializa el LCD
	   LCD_Clear(); //Limpia la pantalla LCD
	   LCD_OutString("Hola Mundo"); //Escribe una cadena de caracteres en las pantalla LCD
	   while(1);
}

/*
 MAIN.c
 Autor: Juan M. Mendoza
 Fecha: Julio 2017
 Descripcion: Programa para usar pantalla LCD Nokia5110 con la libreria Nokia5110.h
 */
/*
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Nokia5110.h"

int main(void) {

	  PLL_Init();                           // set system clock to 50 MHz
	  Nokia5110_Init();
	  for(count=0; count<5; count=count+1){
	    Nokia5110_DrawFullImage(Longhorn);
	    Delay(16666667);                    // delay ~1 sec at 50 MHz
	    Nokia5110_DrawFullImage(Longhorn2);
	    Delay(16666667);                    // delay ~1 sec at 50 MHz
	  }
	  count = 0;
	  Nokia5110_Clear();

	  Nokia5110_OutString("************* LCD Test *************Letter: Num:------- ---- ");
	  Nokia5110_OutChar(127);               // print UT sign
	  while(1){
	            // five leading spaces, bottom row
	    Nokia5110_OutChar((count%26)+'A');
	    Nokia5110_OutChar(' ');
	    Nokia5110_OutUDec(count);
	    Delay(8333333);                     // delay ~0.5 sec at 50 MHz
	    count = count + 1;
	   LCD_Init(); //Inicializa el LCD
	   LCD_Clear(); //Limpia la pantalla LCD
	   LCD_OutString("Hola Mundo"); //Escribe una cadena de caracteres en las pantalla LCD
	   while(1);
}
*/
