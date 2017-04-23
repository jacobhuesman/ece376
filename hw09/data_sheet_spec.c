#include <pic18.h>


unsigned long int TIME, TIME0, TIME1, dT;
unsigned char TRIG_START, TRIG_END;


// Serial port stuff
void SCI_Init(void) {
  TRISC = TRISC | 0xC0;
  TXIE = 0;
  RCIE = 0;
  BRGH = 1;
  BRG16 = 1;
  SYNC = 0;
  SPBRG = 255;
  TXSTA = 0x22;
  RCSTA = 0x90;
}



void interrupt IntServe(void) {
  if (TMR0IF) {
    TMR0       = -37500;
    TMR3       = -100;
    TRIG_START = 1;
    RC3        = 1;
    TMR0IF     = 0;
  } else if (TMR3IF && TRIG_START) {
    TMR3       = -10000; // Interrupting every 100 clock cycles is excessive.
    TRIG_START = 0;
    TMR3IF     = 0;
    RC3        = 0;
  } else if (TMR3IF) {
    TMR3       = -10000;
    TMR3IF     = 0;
  }
}

void Serial_Out(unsigned int DATA) {
  unsigned char A[10], i;

  for (i=0; i<10; i++) {
    A[i] = DATA % 10;
    DATA = DATA / 10;
  }
  for (i=10; i>0; i--) {
    while(!TRMT); TXREG = A[i-1] + '0';
  }
  while(!TRMT); TXREG = 13;
  while(!TRMT); TXREG = 10;
}



void main(void) {
  unsigned int mm;

  TRISA = 0;
  TRISB = 0xFF;
  TRISC = 0x04;
  TRISD = 0;
  ADCON1 = 0x0F;

  TIME = 0;

  // Timer0
  // According to the datasheet the HC-SR04 suggested measurement cycle is 60ms.
  // N = (37500)(16) = 600,000 clocks (60 ms)
  T0CS = 0;
  T0CON = 0x83; // PS = 16
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

  // set up Timer1 for PS = 8
  /*TMR1CS = 0;
  T1CON = 0x81;
  TMR1ON = 1;
  TMR1IE = 1;
  TMR1IP = 1;
  PEIE = 1;*/
  // set up Capture1 for rising edges
  /*TRISC2 = 1;
  CCP1CON = 0x05;
  CCP1IE = 1;
  PEIE = 1;*/

  // Initialize other stuff
  TRIG_START = 0;
  TRIG_END   = 0;

  // turn on all interrupts
  GIE = 1;

  //SCI_Init();

  while(1) {
    mm = dT * 0.1715;   // units = 1/10 mm
    //Serial_Out(mm);

    //LCD_Move(0,0);  LCD_Out(dT, 7);
    //LCD_Move(1,0);  LCD_Out(mm, 1);
  }
}
