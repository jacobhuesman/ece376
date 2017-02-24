// Keypad.C
//
//   Read a Keypad on PORTC
//   Functions are
//      *    Push
//      #    +/-

  
// Global Variables

const unsigned char MSG0[21] = "Keypad.C            "; 
const unsigned char MSG1[21] = "                    "; 


// Subroutine Declarations
#include <pic18.h>

// Subroutines
#include        "lcd_portd.c"

char GetKey(void)
{
   int i;
   unsigned char RESULT;
   TRISA = 0xF8;
   RESULT = 0xFF;
   PORTA = 4;
   for (i=0; i<100; i++);
   if (RA6) RESULT = 1;
   if (RA5) RESULT = 4;
   if (RA4) RESULT = 7;
   if (RA3) RESULT = 10;
   PORTA = 2;
   for (i=0; i<100; i++);
   if (RA6) RESULT = 2;
   if (RA5) RESULT = 5;
   if (RA4) RESULT = 8;
   if (RA3) RESULT = 0;
   PORTA = 1;
   for (i=0; i<100; i++);
   if (RA6) RESULT = 3;
   if (RA5) RESULT = 6;
   if (RA4) RESULT = 9;
   if (RA3) RESULT = 11;
   if (RB0) RESULT = 12;
   if (RB1) RESULT = 13;
   if (RB2) RESULT = 14;
   if (RB3) RESULT = 15;
   if (RB4) RESULT = 16;
   PORTA = 0;
   return(RESULT);
}
      
char ReadKey(void)
{
   char X, Y;
   do {
      X = GetKey();
      }   while(X > 20);
   do {
      Y= GetKey();
      }   while(Y < 20);
   Wait_ms(100);  // debounce
   return(X);
   }
 


void LCD_Out(int DATA, unsigned char N)
{
   unsigned char A[5], i;
   
   if (DATA < 0) {
      LCD_Write('-');
      DATA = -DATA;
   }
   else LCD_Write('+');
   
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
   unsigned int i, j;
   int X, Y, Z, T, TEMP;

   TRISA = 0xF8;
   TRISB = 0xFF;
   TRISC = 0xF8;
   TRISD = 0;
   TRISE = 0;
   TRISA = 0;
   ADCON1 = 15;

   LCD_Init();                  // initialize the LCD

   LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]);
   Wait_ms(2000);
   LCD_Inst(1);

   X = 0;
   Y = 0;
   Z = 0;
   T = 0;

   LCD_Move(0,0);  LCD_Write('Y');
   LCD_Move(1,0);  LCD_Write('X');


   while(1) {
      TEMP = ReadKey();

      if (TEMP < 10) X = (X*10) + TEMP;
 
      if (TEMP == 10) {
         T = Z;
         Z = Y;
         Y = X;
         X = 0;
         }   
      if (TEMP == 11) {
         X = -X;
         } 
 
      LCD_Move(1,5);  LCD_Out(X, 0);
      LCD_Move(0,5);  LCD_Out(Y, 0);
      
      }      


   }
