/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_HEADER_H
#define	LCD_HEADER_H
#endif

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#ifndef RS
#define RS PORTEbits.RE0
#endif

#ifndef RW
#define RW RE1
#endif

#ifndef EN
#define EN RE2
#endif

#ifndef D0
#define D0 RD0
#endif

#ifndef D1
#define D1 RD1
#endif

#ifndef D2
#define D2 RD2
#endif

#ifndef D3
#define D3 RD3

#ifndef D4
#define D4 RD4
#endif

#ifndef D5
#define D5 RD5
#endif

#ifndef D6
#define D6 RD6
#endif

#ifndef D7
#define D7 RD7
#endif

#include <xc.h> // include processor files - each processor file is guarded.  

//LCD Functions Developed by electroSome


void Lcd_Port(char a);

void Lcd_Cmd(char a);

void Lcd_Clear(void);

void Lcd_Set_Cursor(char a, char b);

void Lcd_Init(void);

void Lcd_Write_Char(char a);

void Lcd_Write_String(char *a);

void Lcd_Shift_Right(void);

void Lcd_Shift_Left(void);

#endif	/* LCD_H */

