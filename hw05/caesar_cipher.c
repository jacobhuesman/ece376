/* 
 * Assignment 4 Part 3
 * Nick Carlson, Jacob Huesman
 */


#include <pic18.h>
#include "lcd_portd.c"
 
#define RB0 PORTBbits.RB0
#define RB1 PORTBbits.RB1
#define RB2 PORTBbits.RB2


void wait(unsigned int time) {
  for (unsigned int i=0; i<time; i++) {
    for (int j=0; j<746; j++);
  }
}

void lcd_write_number(unsigned long number, char size, char decimal_index) {
  char output[5];
  
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

void lcd_write_string(char* output, char size) {
  LCD_Move(1,0);
  for (unsigned char i=0; i<size; i++) {
    LCD_Write(output[i]);
  }
}

char get_key(void) {
  char result, i;
  result = 0xFF;
  
  PORTC = 4;
  for (i=0; i<100; i++);
  if (PORTC & 0b01000000) result = 1;
  if (PORTC & 0b00100000) result = 4;
  if (PORTC & 0b00010000) result = 7;
  if (PORTC & 0b00001000) result = 10;
  
  PORTC = 2; 
  for (i=0; i<100; i++);
  if (PORTC & 0b01000000) result = 2;
  if (PORTC & 0b00100000) result = 5;
  if (PORTC & 0b00010000) result = 8;
  if (PORTC & 0b00001000) result = 0;
  
  PORTC = 1;
  for (i=0; i<100; i++);
  if (PORTC & 0b01000000) result = 3;
  if (PORTC & 0b00100000) result = 6;
  if (PORTC & 0b00010000) result = 9;
  if (PORTC & 0b00001000) result = 11;
  if (PORTB & 0b00000001) result = 12;
  if (PORTB & 0b00000010) result = 13;
  if (PORTB & 0b00000100) result = 14;
  if (PORTB & 0b00001000) result = 15;
  if (PORTB & 0b00010000) result = 16;
  PORTC = 0;
  
  return(result);
}
      
char read_key(void) {
  char x,y;
  do {
    x = get_key();
  } while(x > 20);
  do {
    y = get_key();
  } while(y < 20);
  wait(100);  // debounce
  return(x);
}


// Courtesy of Marc Olberding
void write_string(char* string) {
  LCD_Move(0,0);
  while (*string) {
    LCD_Write(*(string++));
  }
}


void read_string(void) {
  // Start shit up
  char index, char1, char2, letter;
  char string[17] = "t               ";
  
  index = 0;
  char1 = read_key();
  while (index < 16) {
    if (char1 < 10) {
      string[index] = (char1-1)*3 + 'a';
      
      write_string(string);
      char2 = read_key();
      while (char2 == char1) {
        if ((string[index] - ((char1-1)*3 + 'a')) < 2) {
          string[index]++;
        } else {
          string[index] = string[index] - 2;
        }
        write_string(string);
        char2 = read_key();
      }
      
      index++;
      char1 = char2;

    }
  }
}

void initialize(void) {
  // Set registers
  TRISA = 0;
  TRISB = 0xFF;
  TRISC = 0xF8;
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

  // Send title to LCD
  const char title[16] = "caesar_cipher.c";
  write_string(title);
 
   
  // Prompt user
  //lcd_write_string("enter message", 13);


  // States:
  // - Displaying "Enter Message" and displaying input
  //    - Cycle through letters
  // - Displaying "Enter Key" and displaying input
  // - Displaying on first line encrypted message and keys below it
  while (1) {
    read_string();

  //if (temp < 10) LCD_Write(temp + '0');

  }
}
    
