#include <pic18.h>

// Global Variables
unsigned long int TIME, TIME0, TIME1, dT;

// Interrupt Service Routine

void interrupt IntServe(void) 
{
   if (TMR0IF) {
      RC3 = !RC3;
      TMR0IF = 0;
      }   
   if (TMR1IF) {
      TIME = TIME + 0x10000;
      TMR1IF = 0;
      }   
   if (CCP1IF) {
      if (CCP1CON == 0x05) {  // rising edge
         TIME0 = TIME + CCPR1;
         CCP1CON = 0x04;
         }
      else {
         TIME1 = TIME + CCPR1;
         dT = TIME1 - TIME0;
         CCP1CON = 0x05;
      }
      CCP1IF = 0;
      }   
   }

// Subroutines
#include        "lcd_portd.c"


void LCD_Out(unsigned long int DATA, unsigned char N)
{
   unsigned char A[10], i;
   
   for (i=0; i<10; i++) {
      A[i] = DATA % 10;
      DATA = DATA / 10;
   }
   for (i=10; i>0; i--) {
      if (i == N) LCD_Write('.');
      LCD_Write(A[i-1] + '0');
   }
}



// Main Routine

void main(void)
{
   int mm;

   TRISA = 0;
   TRISB = 0xFF;
   TRISC = 0x04;	// capture every rising edge
   TRISD = 0;
   ADCON1 = 0x0F;


   LCD_Init();
   Wait_ms(100);

   TIME = 0;


// set up Timer0 for PS = 1
   T0CS = 0;
   T0CON = 0x81;
   TMR0ON = 1;
   TMR0IE = 1;
   TMR0IP = 1;
   PEIE = 1; 
// set up Timer1 for PS = 8
   TMR1CS = 0;
   T1CON = 0x81;
   TMR1ON = 1;
   TMR1IE = 1;
   TMR1IP = 1;
   PEIE = 1;
// set up Capture1 for rising edges
   TRISC2 = 1;
   CCP1CON = 0x05;
   CCP1IE = 1;
   PEIE = 1;

// turn on all interrupts
   GIE = 1;
   
   while(1) {
      mm = dT * 0.1715;   // units = 1/10 mm

      LCD_Move(0,0);  LCD_Out(dT, 7);
      LCD_Move(1,0);  LCD_Out(mm, 1);

      }
   }
