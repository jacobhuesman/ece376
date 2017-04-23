#include <pic18.h>
#include "lcd_portd.c"


unsigned long int TIME, TIME0, TIME1, dT;
unsigned char TRIG_START;


void interrupt IntServe(void) {
  if (TMR0IF) {
    TMR0       = -41666;
    TMR3       = -100;
    TRIG_START = 1;
    RC3        = 1;
    TMR0IF     = 0;
  } else if (TMR3IF && TRIG_START) {
    TMR3       = 0; // Interrupting every 100 clock cycles is excessive.
    TRIG_START = 0;
    TMR3IF     = 0;
    RC3        = 0;
  } else if (TMR3IF) {
    TMR3       = -10000;
    TMR3IF     = 0;
  }
  if (CCP1IF) {
    if (CCP1CON == 0x05) { // Capture rising edge
      CCP1CON = 0x04;      // Set to capture falling edge
    } else {               // Capture falling edge
      dT = CCPR1 * 2;      // Counts every fourth clock cycle, but we only want half the time.
      CCP1CON = 0x05;      // Set to capture rising edge
    }
    TMR1   = 0;
    CCP1IF = 0;
  }
  if (TMR1IF) {
    TMR1   = 0;
    TMR1IF = 0;
  }
}

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


void main(void) {
  unsigned long d;

  TRISA = 0;
  TRISB = 0xFF;
  TRISC = 0x04;
  TRISD = 0;
  ADCON1 = 0x0F;

  LCD_Init();
  Wait_ms(100);

  TIME = 0;

  // Timer0
  // N = (41,666)(8) = 333,333 clocks (~30 Hz)
  T0CS = 0;
  T0CON = 0x82; // PS = 8
  TMR0ON = 1;
  TMR0IE = 1;
  TMR0IP = 1;
  PEIE = 1; 

  // Timer3
  // N = (100)(1) = 100 clocks (10 us)
  TMR3CS = 0;
  T3CON = 0x81; // PS = 1
  TMR3ON = 1;
  TMR3IE = 1;
  TMR3IP = 1;
  PEIE = 1; 

  // set up Timer1 for PS = 4
  TMR1CS = 0;
  T1CON = 0xA1;
  TMR1ON = 1;
  TMR1IE = 1;
  TMR1IP = 1;
  PEIE = 1;

  // set up Capture1 to capture a rising edge
  CCP1CON = 0x05;
  CCP1IE = 1;
  PEIE = 1;

  // Initialize other stuff
  TRIG_START = 0;

  // turn on all interrupts
  GIE = 1;

  //SCI_Init();

  while(1) {
    d = dT * 343;

    LCD_Move(0,0);  LCD_Out(dT, 1);
    LCD_Move(1,0);  LCD_Out(d, 5);
  }
}
