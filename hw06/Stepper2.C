// Stepper2.C
// Drive a stepper motor at 500ms/step

// Global Variables
const unsigned char MSG[16] = "Stepper2.C      ";
unsigned char TABLE[4] = {1, 2, 4, 8};
int STEP, N, TIME;

// Subroutine Declarations
#include <pic18.h>
#include "LCD_PortD.C"

// High-priority service
void interrupt IntServe(void)
{
   if (TMR2IF) {
      TIME = TIME + 1;
      N = (N + 1) % 100;
      if (N == 0) {
         STEP = STEP + 1;
         PORTC = TABLE[STEP %  4];
         }
      TMR2IF = 0;
      }
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
   int i;

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

// initialize Timer2

   T2CON = 0x4D;
   PR2 = 249;
   TMR2ON = 1;
   TMR2IE = 1;
   PEIE = 1;

   GIE = 1;

   while(1) {
      LCD_Move(1,0);   LCD_Out(TIME,3);
      LCD_Move(1,8);   LCD_Out(STEP,0);
      }
  
 }      

