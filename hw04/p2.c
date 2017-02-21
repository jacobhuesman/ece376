/* 
 * Assignment 4 Part 2
 * Jacob Huesman, Nick Carlson
 */


#include <pic18.h>
#include "lcd_portd.c"


void lcd_out(unsigned long number) {
  unsigned char output[10];
  
  for (int i=0; i<10; i++) {
    output[i] = number % 10;
    number    = number / 10;
  }

  for (int i=10; i>0; i--) {
    LCD_Write(output[i-1] + '0');
  }

}

void main(void) {
  TRISD = 0;
  PORTD = 0;
  LCD_Init();

  // Send title to LCD
  const unsigned char MSG0[16] = "p2.c            ";
  LCD_Move(0,0);
  for (int i=0; i<20; i++) {
    LCD_Write(MSG0[i]);
  }

  // Count on LCD
  unsigned long count = 0;
  while(1) {
    LCD_Move(1,0);
    lcd_out(count++);
    PORTDbits.RD0++;
  }
}
