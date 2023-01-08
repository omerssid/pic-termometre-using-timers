
#define _XTAL_FREQ 4000000
#include <xc.h>
#include"pic16f877a.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define ohm 0xf4                    
#define B_coefficient 3950.00      /* B coefficient of NTC Thermistor*/
#define Room_temperature 95.00
#define Series_Resistance 680.00

float Get_Temperature(int);
float Termometre_temp;
long NTC_Resistance;
int Temperature = 0;
unsigned int x;
unsigned char i = 0, j=0, a =0;
unsigned char display[]={0x3f, 0x06, 0x5B, 0x4F, 0x66,0x6D,0x7D,0x07,0x7F, 0x6F};
unsigned char yuzler = 0,onlar = 0, birler = 0;
unsigned char disp_hane[3];
 

 
unsigned int ADC_oku(unsigned char kanal)
{
    if(kanal>7)
        return 0;
    ADCON0 &= 0b11000101;
    ADCON0|= kanal<<3; // 111 111000
    __delay_ms(2);
    GO_nDONE=1;
    while(GO_nDONE);
    return ((ADRESH<<8)+ADRESL);
}


 float Get_Temperature(int analog_value)
{
    analog_value = ADC_oku(0);		/* store adc value on val register */

    /* calculate the NTC resistance */
	NTC_Resistance = ((1023*Series_Resistance/analog_value) - Series_Resistance);			
	 Termometre_temp = log(NTC_Resistance);	/* calculate natural log of resistance */

    /* Calculate Temperature using B parameter Equation */
    /* 1/T = 1/T0 + ((1/B_coefficient)*log(NTC_Resistance/Series_Resistance)) */
     Termometre_temp = (1.0/(Room_temperature + 273.15))+ ((1.0/B_coefficient)*log(NTC_Resistance/Series_Resistance));
     Termometre_temp = (1/ Termometre_temp) - 273.15;	/* convert kelvin to °C */
    
	return  Termometre_temp;
    
}


void main(void) {  
    TRISA=0X01;
    TRISB=0X00;
    TRISC=0X00;
    TRISD=0X00;
    
    T0CS = 0; PSA = 0; PS2 = 1; PS1 = 0; PS0 = 1;
    TMR0 =  256-156; TMR0IF = 0; TMR0IE = 1;
    GO = 0;
    ADIF = 0; ADIE = 1;
    GIE = 1; PEIE = 1;
    ADCON0=0B10000001;
    ADCON1=0B10000000;
    while(1){      
		x = ADC_oku(0);	
		Temperature = Get_Temperature(x);
        __delay_ms(100);
    }
    
           
}

 void __interrupt() KESME(void) {
     if(TMR0IF){
         TMR0 = 256-156;
          if(Temperature < 80) {
             disp_hane[0] = 0x38;
             disp_hane[1] = 0x3F;
             disp_hane[2] = 0x3E;
         } else if (Temperature > 110) {
             disp_hane[0] = 0x76;
             disp_hane[1] = 0x76;
             disp_hane[2] = 0x76;
         } else {
             disp_hane[0] = display[yuzler];
             disp_hane[1] = display[onlar];
             disp_hane[2] = display[birler];
         }
            if( i== 1) { PORTC = 0x0E; PORTB = disp_hane[0]; }
            if( i== 2) { PORTC = 0x0D; PORTB = disp_hane[1]; }
            if( i== 3) { PORTC = 0x0B; PORTB = disp_hane[2]; i = 0; } 
            i++;

        TMR0IF = 0;
     }
     if( ADIF ) {
         a = (int)Temperature;
         yuzler = a / 100;
         a = a % 100;
         onlar = a / 10;
         birler = a % 10;
         ADIF = 0;
     }
     GIE = 0;
 }
 
