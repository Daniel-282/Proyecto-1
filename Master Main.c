//******************************************************************************
/* 
 * File:   Master Main.c
 * Author: Daniel
 *
 * Created on August 30, 2021, 11:34 AM
 */
//******************************************************************************
//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = XT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include "LCD header.h"
#include <xc.h>
#include <stdio.h>
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
#define l1 RA4
#define l2 RA5
#define l3 RA6
#define l4 RA7

unsigned int dc = 30;

int envio1 = 0xFF;
int envio2 = 0x00;
int bandera1 = 0;
int bandera2 = 0;
int banderaSLAVE1 = 0;
int banderaSLAVET = 0;
__bit ANTIREBOTE_CONTADOR1;
__bit ANTIREBOTE_CONTADOR2;
char StringConversion[8];
int ValorContador;
unsigned int ValorADC;
unsigned int temp;

int contadorNum = 0;
int contadorDec = 0;
int contadorDec2 = 0;

int Temperatura;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
//*****************************************************************************
    if(RBIF == 1){
        //BOTON RESTART
        if ((((RA0 == 0 && RA1 == 1) && ValorContador == 0) && RA2 == 0) && RB0 == 1){
            ANTIREBOTE_CONTADOR1 = 1;
        }
        if (ANTIREBOTE_CONTADOR1 == 1 && RB0 == 0){
            ANTIREBOTE_CONTADOR1 = 0;
            RA0 = 1;
            RA1 = 0;
            RA3 = 0;
            RA4 = 0;
            envio2 = 1;
        }
        //BOTON RESET
        if (((ValorContador == 0 && RA2 == 1) && RB1 == 1) && banderaSLAVET == 0){
            ANTIREBOTE_CONTADOR2 = 1;
         }
        if (ANTIREBOTE_CONTADOR2 == 1 && RB1 == 0){
            ANTIREBOTE_CONTADOR2 = 0;
            RA0 = 1;
            RA1 = 0;
            RA2 = 0;
            RA3 = 0;
            RA4 = 0;
            envio2 = 1;
        }
        
        RBIF = 0;
    }
//*****************************************************************************    
}
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
//*************************Interfaz grafica para la LCD*************************      
    Lcd_Init();
  
        Lcd_Clear(); 
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("S1");
        Lcd_Set_Cursor(1,8);
        Lcd_Write_String("S2");
        Lcd_Set_Cursor(1,13);
        Lcd_Write_String("S3");
        Lcd_Set_Cursor(2,5);
        Lcd_Write_String("V");
        Lcd_Set_Cursor(2,16);
        Lcd_Write_String("C");
//***************************Condiciones iniciales******************************
        RA0 = 1;
        RA1 = 0;
        RA2 = 0;
        l1 = 1;
        l2 = l3 = l4 = 0;
        __delay_ms(100);
//*************************Inicialización Sensor I2C****************************
   I2C_Master_Start();
   I2C_Master_Write(0x90|(0x03<<1)); //Genera primer byte (1001A2A1A0-W)
   I2C_Master_Write(0xee);        //Lee temperatura
   I2C_Master_Stop();
   __delay_ms(100);
        
        
    while(1){
//**********************Escritura y Lectura Sensor I2C**************************         
        I2C_Master_Start();
        I2C_Master_Write(0x90|(0x03<<1)); //Genera primer byte (1001A2A1A0-W)
        I2C_Master_Write(0xee);        //Lee temperatura
        I2C_Master_Stop();
        __delay_ms(100);

        I2C_Master_Start();
        I2C_Master_Write(0x90|(0x03<<1)); //Genera primer byte (1001A2A1A0-W)
        I2C_Master_Write(0xaa);        //Lee temperatura
        I2C_Master_Stop();
        __delay_ms(100);
        I2C_Master_Start();
        I2C_Master_Write(0x91|(0x03<<1)); //Genera primer byte (1001A2A1A0-R)
        Temperatura = I2C_Master_Read(0); 
        I2C_Master_Stop();
         __delay_ms(100);
        
//************************Escritura y Lectura Slave 1***************************
        I2C_Master_Start();
        I2C_Master_Write(0x50);
        I2C_Master_Write(envio1);
        I2C_Master_Stop();
        __delay_ms(100);
       
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        ValorADC = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(100);
//*************************Escritura y Lectura Slave 2**************************       
        I2C_Master_Start();
        I2C_Master_Write(0x60); 
        I2C_Master_Write(envio2);
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_Start();
        I2C_Master_Write(0x61);
        ValorContador = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200); 
//******************************************************************************
//  CONTROL DE MOTORES   
//******************************************************************************
        if(Temperatura > 27 || (Temperatura < 10 && Temperatura > 0)){
            banderaSLAVET = 1;
        }
        else if(Temperatura <= 27 || Temperatura >= 10){
            banderaSLAVET = 0;
        }
//******************************************************************************
        if(ValorADC > 89){
            banderaSLAVE1 = 1;
        }
        else if(ValorADC <= 89){
            banderaSLAVE1 = 0;
        }
//******************************************************************************
        if (((RA0 == 1 && RA1 == 0) && ValorContador == 0) && RA2 == 0){
            RA3 = 0;
            l1 = 1;
            l2 = l3 = l4 = 0;
            __delay_ms(100);
        }
        if (((RA0 == 1 && RA1 == 0) && ValorContador == 1) && RA2 == 0){
            RA3 = 1;
            l1 = 1;
            l2 = l3 = l4 = 0;
            __delay_ms(100);
            envio2 = 0;
            bandera1 = 1;
        }
        if ((((RA0 == 1 && RA1 == 0) && ValorContador == 0) && RA2 == 0) && bandera1 == 1){
            RA3 = 1;
            l1 = 1;
            l2 = l3 = l4 = 0;
            __delay_ms(100);
            envio2 = 0;
            bandera1 = 0;
        }
        if ((((RA0 == 1 && RA1 == 0) && ValorContador == 2) && RA2 == 0) && banderaSLAVE1 == 0){
            RA3 = 0;
            l1 = 0;
            l2 = 1;
            l3 = l4 = 0;
            __delay_ms(100);
            envio2 = 0;
            bandera2 = 1;
        }
        if ((((RA0 == 1 && RA1 == 0) && ValorContador == 2) && RA2 == 0) && banderaSLAVE1 == 1){
            RA3 = 1;
            l1 = 1;
            l2 = l3 = l4 = 0;
            __delay_ms(100);
            envio2 = 0;
            bandera2 = 1;
        }
        if ((((RA0 == 1 && RA1 == 0) && ValorContador == 0) && RA2 == 0) && bandera2 == 1){
            RA3 = 1;
            l1 = 1;
            l2 = l3 = l4 = 0;
            __delay_ms(100);
            envio2 = 0;
            bandera2 = 0;
        }
        if (((RA0 == 1 && RA1 == 0) && ValorContador == 3) && RA2 == 0){
            RA0 = 0;
            RA1 = 1;
            RA3 = 0;
            l1 = 1;
            l2 = l3 = l4 = 0;
            __delay_ms(100);
            envio2 = 0;
        }
        if (ValorContador == 4 || banderaSLAVET == 1){
            RA0 = 0;
            RA1 = 0;
            RA2 = 1;
            RA3 = 0;
            l1 = 1;
            l2 = l3 = l4 = 0;
            __delay_ms(100);
            envio2 = 0;
        }
//******************************************************************************
//  SENSORES DE PROXIMIDAD LCD    
//******************************************************************************
        Lcd_Set_Cursor(2,8);
        sprintf(StringConversion, "%d", ValorContador);    //convierte contador a string
        Lcd_Write_String(StringConversion);               //Imprime contador o sensor 2
        
        if (ValorContador >= 0 ){                //Condiciones de seguridad para borrar ceros indeseados al incrementar y decrementar contador
            if (ValorContador< 10){
                Lcd_Set_Cursor(2,9);
                Lcd_Write_String(" ");
                Lcd_Set_Cursor(2,10);
                Lcd_Write_String(" ");
            }
        }
        if (ValorContador >= 10 ){
            if (ValorContador < 100){
                Lcd_Set_Cursor(2,10);
                Lcd_Write_String(" ");
            }
        }
        if (ValorContador <= 0 ){
            if (ValorContador > -10){
                Lcd_Set_Cursor(2,10);
                Lcd_Write_String(" ");
            }
        }
        if (ValorContador <= -10 ){
            if (ValorContador > -100){
                Lcd_Set_Cursor(2,11);
                Lcd_Write_String(" ");
            }
        }
        __delay_ms(5); 
//******************************************************************************
//  SENSOR TEMPERATURA LCD     
//******************************************************************************
        Lcd_Set_Cursor(2,13);
        sprintf(StringConversion, "%d", Temperatura);    //convierte contador a string
        Lcd_Write_String(StringConversion);               //Imprime contador o sensor 2 

        if (Temperatura >= 0 ){                //Condiciones de seguridad para borrar ceros indeseados al incrementar y decrementar contador
            if (Temperatura< 10){
                Lcd_Set_Cursor(2,14);
                Lcd_Write_String(" ");
                Lcd_Set_Cursor(2,15);
                Lcd_Write_String(" ");
            }
        }
        if (Temperatura >= 10 ){
            if (Temperatura < 100){
                Lcd_Set_Cursor(2,15);
                Lcd_Write_String(" ");
            }
        }
        if (Temperatura <= 0 ){
            if (Temperatura > -10){
                Lcd_Set_Cursor(2,15);
                Lcd_Write_String(" ");
            }
        }
        if (Temperatura <= -10 ){
            if (Temperatura > -100){
                Lcd_Set_Cursor(2,16);
                Lcd_Write_String(" ");
            }
        }
        __delay_ms(5);         
//******************************************************************************
//  SENSOR ULTRASONICO LCD      
//******************************************************************************     
        Lcd_Set_Cursor(2,1);                    
        temp = ValorADC;     //Conversion completa ADC
        sprintf(StringConversion, "%d", temp);
        Lcd_Write_String(StringConversion);
//******************************************************************************
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0xFF;
    TRISCbits.TRISC2 = 0;
    TRISD = 0;
    TRISE = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones perifericas
//*****************************************************************************    
    TMR2 = 155;
    PR2 = 255;
    T2CONbits.TOUTPS = 0b1111;      //POSTSCALER 1:16
    T2CONbits.T2CKPS = 0b11;        //Prescaler 1:16
    T2CONbits.TMR2ON = 1;           //tmr2 on 
    
    CCP1CON = 0B00001100;
    
    CCP1CONbits.CCP1Y = dc & (1<<0);
    CCP1CONbits.CCP1X = dc & (1<<1);
    CCPR1L = dc>>2;
//*****************************************************************************    
    IOCB0 = 1; //Habilita interrupción por cambio
    IOCB1 = 1;
    
    RBIE=1;    //Habilita interrupción puerto B
    RBIF=0;
    
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
    
    IRCF0 = 1; //Oscilador a 8MHz
    IRCF1 = 1;
    IRCF2 = 1;
    SCS = 1;
}