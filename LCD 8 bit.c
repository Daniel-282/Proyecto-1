#include "LCD header.h"

void Lcd_Port(char a) {
    if (a & 1)
        D0 = 1;
    else
        D0 = 0;

    if (a & 2)
        D1 = 1;
    else
        D1 = 0;

    if (a & 4)
        D2 = 1;
    else
        D2 = 0;

    if (a & 8)
        D3 = 1;
    else
        D3 = 0;
    
    if (a & 16)
        D4 = 1;
    else
        D4 = 0;

    if (a & 32)
        D5 = 1;
    else
        D5 = 0;

    if (a & 64)
        D6 = 1;
    else
        D6 = 0;

    if (a & 128)
        D7 = 1;
    else
        D7 = 0;    
}

void Lcd_Cmd(char a) {
    RS = 0; // => RS = 0 // Dato en el puerto lo va interpretar como comando
    Lcd_Port(a);
    EN = 1; // => E = 1
    __delay_ms(4);
    EN = 0; // => E = 0
}

void Lcd_Clear(void) {
    Lcd_Cmd(0);
    Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b) {
    char c;
    if (a == 1) {
        c = 0x80 + b - 1;
        Lcd_Cmd(c);
    } else if (a == 2) {
        c = 0xC0 + b - 1;
        Lcd_Cmd(c);
    }
}

void Lcd_Init(void) {
    Lcd_Port(0x00);
    __delay_ms(20);
    Lcd_Cmd(0x030);
    __delay_ms(5);
    Lcd_Cmd(0x030);
    __delay_ms(11);
    Lcd_Cmd(0x030);
    /////////////////////////////////////////////////////
    Lcd_Cmd(0x38);
    Lcd_Cmd(0x08);//aqui se apaga display
    Lcd_Cmd(0x01);//aqui se limpia el display
    __delay_ms(11);
    Lcd_Cmd(0x06);
    Lcd_Cmd(0x0C);//aqui encendemos display
}

void Lcd_Write_Char(char a) {
    RS = 1; // => RS = 1
    Lcd_Port(a); //Data transfer
    EN = 1;
    __delay_ms(40);
    EN = 0;
}

void Lcd_Write_String(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++)
        Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Right(void) {
    Lcd_Cmd(0x1C);
}

void Lcd_Shift_Left(void) {
    Lcd_Cmd(0x18);
}

