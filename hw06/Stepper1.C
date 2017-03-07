// Stepper1.C
// Drive a stepper motor at 100ms/step

// Global Variables
const unsigned char MSG[16] = "Stepper1.C      ";
unsigned char TABLE[4] = {1, 2, 4, 8};

// Subroutine Declarations
#include <pic18.h>
#include "LCD_PortD.C"

// High-priority service

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
   int i, STEP;

   TRISA = 0;
   TRISB = 0;
   TRISC = 0;
   TRISD = 0;
   TRISE = 0;
   ADCON1 = 0x0F;

   STEP = 0;

   LCD_Init();
   LCD_Move(0,0);
   for (i=0; i<16; i++) LCD_Write(MSG[i]);
   Wait_ms(100);

   while(1) {
      STEP = STEP + 1;
      PORTC = TABLE[STEP % 4];
    //  LCD_Move(1,0);
     // LCD_Out(STEP,0);
      Wait_ms(10);
      }
  
 }      

