#include <pic18.h>
#include "lcd_portd.c"

unsigned char COUNTDOWN, MEASUREMENTS;
unsigned long int SUM, TOT;
unsigned int AVE;


unsigned int A2D_Read(unsigned char c) {
  unsigned int result;
  unsigned char i;
  c = c & 0x0F;
  ADCON0 = (c << 2) + 0x01; // set Channel Select
  for (i=0; i<20; i++); // wait 2.4us (approx)
  GODONE = 1; // start the A/D conversion
  while(GODONE); // wait until complete (approx 8us)
  return(ADRES);
}   

void interrupt IntServe(void) {
  if (TMR0IF) {
    TMR0 = -39063;
    RC1 = !RC1;
    if(COUNTDOWN){
      COUNTDOWN--;
      if(!COUNTDOWN){
        MEASUREMENTS = 60;
        SUM = 0;
        TMR1IF = 1;
      }
    }
    TMR0IF = 0;
  }
  if (TMR1IF) {
    TMR1 = -62500;
    if(MEASUREMENTS){
      MEASUREMENTS--;
      SUM = SUM + A2D_Read(0);
    }
    else{
      TOT = SUM;
    }
    RC0 = !RC0;
    TMR1IF = 0;
  }
}

void LCD_Out(unsigned long int DATA, unsigned char N) {
  unsigned char A[10], i;

  for (i=0; i<10; i++) {
    A[i] = DATA % 10;
    DATA = DATA / 10;
  }
  for (i=5; i>0; i--) {
    if (i == N) LCD_Write('.');
    LCD_Write(A[i-1] + '0');
  }
}
 

void main(void) {
  COUNTDOWN = 0;
  SUM = 0;
  MEASUREMENTS = 0;
  AVE = 0;

  TRISA = 0xFF;
  TRISC = 0;
  PORTC = 0;
  TRISD = 0;
  ADCON2 = 0x85;
  ADCON1 = 0x07;
  ADCON0 = 0x01;

  LCD_Init();
  Wait_ms(100);

  // Timer0
  // N = (39,063)(256) ~= 10,000,000 clocks
  T0CS = 0;
  T0CON = 0x87; // PS = 256
  TMR0ON = 1;
  TMR0IE = 1;
  TMR0IP = 1;
  PEIE = 1; 

  // set up Timer1 for PS = 8
  // N = (62,500)(8) ~= 500,000 clocks
  TMR1CS = 0;
  T1CON = 0xB1;
  TMR1ON = 1;
  TMR1IE = 1;
  TMR1IP = 1;
  PEIE = 1;

  // turn on all interrupts
  GIE = 1;

  //SCI_Init();
  unsigned int i;
  const unsigned char MSG0[21] = "Countdown:         "; 
  LCD_Move(0,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]);
  
  const unsigned char MSG0[21] = "BAC:               "; 
  LCD_Move(1,0);  for (i=0; i<20; i++) LCD_Write(MSG0[i]);
  
  unsigned int BAC, Vout;
  BAC = 0;
  
  while(1) {
    if(RB0){
      COUNTDOWN = 6;
      TMR0IF = 1;
      Wait_ms(5);
    }

    //TOT = SUM / 60;
    //BAC = TOT * 42 / 12276;
    //BAC = 11.721 * TOT / 60 - 3.6;
    Vout = ((5.0 / 1023.0 * TOT / 60.0) - 1.5) / 41.7 * 1000;
    
    
    LCD_Move(0,11);  LCD_Out(COUNTDOWN, 0);
    LCD_Move(1,11);  LCD_Out(Vout, 4);
  }
}
