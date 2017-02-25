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

void write_char(unsigned char number, char row) {
  char output[3];
  LCD_Move(row, 0);
  
  for (int i=0; i<3; i++) {
    output[i] = number % 10;
    number    = number / 10;
  }

  for (int i=3; i>0; i--) {
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
void write_string(char* string, char row) {
  LCD_Move(row,0);
  char i = 0;
  while (i++ < 16) {
    if (*string) LCD_Write(*(string++));
    else LCD_Write(' ');
  }
}

void empty_string(char* string, char size) {
  char i = 0;
  while (i < size-1) {
    string[i++] = ' ';
  }
  string[size-1] = 0;
}

void nullify(char* string, char size) {
  char i = 0;
  while (i < size) {
    string[i] = 0;
    i++;
  }
}


// Read a t3 encoded string
void read_message(char* string) {
  // Start shit up
  char index, char1, char2, letter, space;

  write_string("Enter message:", 0);
  PORTB = PORTB & 0b00001111;
  empty_string(string, 17);
  index = 0;
  space = 0;
  char1 = read_key();
  while (index < 16) {
    if (char1 < 10) {
      string[index] = (char1-1)*3 + 'a';
      
      write_string(string, 0);
      char2 = read_key();
      while (char2 == char1) {
        if ((string[index] - ((char1-1)*3 + 'a')) < 2) {
          string[index]++;
        } else {
          string[index] = string[index] - 2;
        }
        write_string(string, 0);
        char2 = read_key();
      }
      
      index++;
      char1 = char2;
      space = 0;
    } else if (char1 < 12) {
      if (char1 == 10) { // User pressed *
        string[--index] = ' ';
      } else if (char1 == 11) { // User pressed #
        if (space == 0) {
          space++;
        } else {
          index++;
          space = 0;
        }
      }
      write_string(string, 0);
      char1 = read_key();
    } else if (char1 < 16) {
      if (char1 == 12) {
        PORTB = PORTB | 0b10000000;
        return;
      }
    }
  }
}

// Assumes the array has at least one character in it
unsigned char array_to_number(char* array) {
  char index             = 3;
  unsigned char number   = 0;
  unsigned char position = 10;
  while(!array[index]) index--;
  number = array[index] - '0';
  if (index > 0) {
    index--;
    while(index > 0) {
      unsigned char test = (array[index] - '0') * position;
      index--;
      number = number + test;
      position = position * 10;
    }
    unsigned char test = (array[index] - '0') * position;
    number = number + test;
  } 
  return number;
}

unsigned int read_passkey() {
  // Start shit up
  char index, char1;
  unsigned int key;
  char string[17];

  write_string("Enter key:", 1);
  PORTB = PORTB & 0b00001111;
  
  nullify(string, 17);
  index = 0;
  key = 0;
  
  char1 = read_key();
  while (index < 16) {
    if (char1 < 10) {
      string[index++] = char1 + '0';
      write_string(string, 1);
      char1 = read_key();
    } else if (char1 < 16) {
      if (char1 == 12) {
        PORTB = PORTB | 0b10000000;
        key = array_to_number(string);
        return key;
      }
    }
  }
  key = array_to_number(string);
  return key;
}

void initialize(void) {
  // Set registers
  TRISA = 0;
  TRISB = 0x0F;
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
  write_string("caesar_cipher.c", 0);
  char message[17];
  char key;
 
   
  // Prompt user
  //lcd_write_string("enter message", 13);


  // States:
  // - Displaying "Enter Message" and displaying input
  //    - Cycle through letters
  // - Displaying "Enter Key" and displaying input
  // - Displaying on first line encrypted message and keys below it
  while (1) {
    read_message(message);
    key = read_passkey();
    
    write_string("Hit key any key", 0);
    write_string("to encrypt"     , 1);
    read_key();

    write_string(message, 0);
    write_string(" "    , 1);
    write_char(key, 1);
    read_key();




  //if (temp < 10) LCD_Write(temp + '0');

  }
}
    
