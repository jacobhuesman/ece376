#include <pic18.h>
#include "lcd_portd.c"


unsigned int N;


// High-priority service
void interrupt IntServe(void) {
  if (TMR0IF) {	
    TMR0 = -50000; // 50,000 * 4 = 200,000, 200,000 * 100ns = 20,000,000ns = 20ms
    TMR1 = -N; 
    RC0 = 1;
    TMR0IF = 0;
  }
  if (TMR1IF) { // 1 * N = N ns
    RC0 = 0;
    TMR1IF = 0;
  }
}
   
void LCD_Out(unsigned int DATA, unsigned char N) {
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

unsigned int a2d_read(void) {
  unsigned int result;
  ADCON0 = 0x01; // Select channel 0, turn on, CLK/32
  GODONE = 1;    // Start conversions
  while(GODONE); // wait until done (approx 8us)
  return(ADRES); // and return the result
}


// Main Routine
void main(void) {

  TRISA = 0;
  TRISB = 0xFF;
  TRISC = 0;
  TRISD = 0;
  TRISE = 0;
  ADCON1 = 0x0F;

  LCD_Init();

  const unsigned char MSG0[21] = "Quad Copter         ";
  const unsigned char MSG1[21] = "PW (ms)             ";
  LCD_Move(0,0);  for (int i=0; i<20; i++) LCD_Write(MSG0[i]);
  LCD_Move(1,0);  for (int i=0; i<20; i++) LCD_Write(MSG1[i]);

  Wait_ms(1000);
  N = 9000;

  // set up Timer0 for PS = 4
  T0CS = 0;
  T0CON = 0x81;
  TMR0ON = 1;
  TMR0IE = 1;
  TMR0IP = 1;
  PEIE = 1;
  // set up Timer1 for PS = 1
  TMR1CS = 0;
  T1CON = 0x81;
  TMR1ON = 1;
  TMR1IE = 1;
  TMR1IP = 1;
  PEIE = 1;
  // turn on all interrupts
  GIE = 1;

  while(1) {
    unsigned int result = a2d_read();
    if (result <= 64) {
        N = 9000;
    } else {
       N = 10000 + result/3;
    }
    LCD_Move(1,9);  LCD_Out(N, 4);
  }      
}

