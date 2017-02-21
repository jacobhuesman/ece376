
// Global Variables

const unsigned char MSG0[20] = "LCD_Demo.C          ";

// Subroutine Declarations
#include <pic18.h>

// Subroutines
#include        "lcd_portd.c"
  

void Wait(unsigned int X)
{
   unsigned int i, j;

   for (i=0; i<X; i++)
      for (j=0; j<62; j++);
   }

void LCD_Out(unsigned int DATA, unsigned char N)
{
   unsigned char A[5], i;
   
   for (i=0; i<5; i++) {
      A[i] = DATA % 10;
      DATA = DATA / 10;
   }
   for (i=5; i>0; i--) {
      if (i == N) LCD_Write('.');
      LCD_Write(A[i-1] + '0');
   }
}

 

         
// Main Routine

void main(void)
{
   unsigned int HR, MIN, SEC;
   unsigned int i;

   TRISA = 0;
   TRISB = 0xFF;
   TRISC = 0;
   TRISD = 0;
   TRISE = 0;
   TRISA = 0;
   PORTB = 0;
   PORTC = 0;
   PORTD = 0;
   PORTE = 0;
   ADCON1 = 0x0F;

   LCD_Init();                  // initialize the LCD

   SEC = 0;
   MIN = 1;
   HR = 2;
 
   LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]); 
   Wait_ms(100); 


   i = 0;

   while(1) {
      LCD_Move(1,0); 
      LCD_Out(MIN,0);
      LCD_Write(':');
      LCD_Out(SEC,1);

      SEC = SEC + 1;
      Wait_ms(100);
      }


}
    