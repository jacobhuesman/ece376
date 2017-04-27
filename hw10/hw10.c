#include <pic18.h>
#include "lcd_portd.c"

unsigned char COUNTDOWN, MEASUREMENTS;
unsigned long int SUM;
unsigned int AVE;


unsigned int A2D_Read(unsigned char c)
{
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
    if(COUNTDOWN){
        COUNTDOWN--;
        if(!COUNTDOWN){
            MEASUREMENTS = 120;
            SUM = 0;
            TMR1IF = 1;
        }
    }
    TMR0IF = 0;
  }
  if (TMR1IF) {
      RC0 = !RC0;
      TMR1 = -62,500;
      if(MEASUREMENTS){
        MEASUREMENTS--;
        SUM += A2D_Read(0);
      }
      else{
        AVE = SUM / 120;
    }
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
  COUNTDOWN = 0;
  SUM = 0;
  MEASUREMENTS = 0;
  AVE = 0;

  TRISA = 0xFF;
  TRISC = 0;
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

  while(1) {
      if(RB0){
          COUNTDOWN = 6;
          TMR0IF = 1;
          Wait_ms(5);
      }
      
    LCD_Move(0,0);  LCD_Out(COUNTDOWN, 0);
    LCD_Move(1,0);  LCD_Out(SUM, 0);
  }
}
