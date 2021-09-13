#include <stdint.h>
#include <pic16f887.h>
#include "USART header.h"

char value;
char value2;

void UART_Write_Text(char *text)
{
  int i;
  for(i=0;text[i]!='\0';i++)
   EnvioSerial(text[i]);
}

void EnvioSerial (char value){
    TXREG = value; //Carga al registro de envio el dato deseado
    while (TXSTAbits.TRMT == 0){  //Esperar a que termine envio.
    }
}

void InterruptReciboUSART (int *value2){
    if (RCIF == 1){
        *value2 = RCREG; //lee registro de recepción
        RCIF = 0; //limpia bandera de recepción
    }
}


