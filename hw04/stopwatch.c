/* 
 * Assignment 4 Part 3
 * Nick Carlson, Jacob Huesman
 */


#include <pic18.h>
#include "lcd_portd.c"
 
#define RB0 PORTBbits.RB0
#define RB1 PORTBbits.RB1
#define RB2 PORTBbits.RB2
#define RC0 PORTCbits.RC0


void wait(unsigned int time) {
  for (unsigned int i=0; i<time; i++) {
    for (int j=0; j<746; j++);
  }
}

void lcd_out(unsigned long number, unsigned char size, unsigned char decimal_index) {
  unsigned char output[5];
  
  for (int i=0; i<size; i++) {
    output[i] = number % 10;
    number    = number / 10;
  }

  for (int i=size; i>0; i--) {
    if (i == decimal_index) {
      LCD_Write('.');
    }
    LCD_Write(output[i-1] + '0');
  }
}

void initialize(void) {
  // Set registers
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

  //Initialize LCD
  LCD_Init();
}

         
void main(void)
{
  initialize();

  unsigned int hr, min, sec, run;
  sec = 0;
  min = 0;
  hr  = 0;
         
  // Send title to LCD
  const unsigned char MSG0[16] = "stopwatch.c     ";
  LCD_Move(0,0);
  for (int i=0; i<20; i++) {
    LCD_Write(MSG0[i]);
  }

  while (1) {
    // Write values to LCD
    LCD_Move(1,0);
    lcd_out(hr,2,0);
    LCD_Write(':');
    lcd_out(min,2,0);
    LCD_Write(':');
    lcd_out(sec,3,1);
    
    // Update values
    if (run == 1) {
      if (sec == 600) {
          min = min +1;
          sec = 0;
      }
      if (min == 60){
          hr  = hr + 1;
          min = 0;
      }

      sec++;
      RC0++;
      wait(100);
    }

    // Manage input
    if (RB0 == 1) {
      run = 1;
    }
    if (RB1 == 1) {
      run = 0;
    }
    if (RB2 == 1) {
      hr  = 0;
      min = 0;
      sec = 0;
    }
  }
}
    
