// 
// Homework 7
// Jacob Huesman, Nick Carlson
//


#include <pic18.h>
#include "lcd_portd.c"


// Global Variables
unsigned char TABLE[4] = {1, 2, 4, 8};
int DIR, SPEED, STEP, N, TIME;


// High-priority service. 
// Called every 1ms. 
// 200 steps per rotation
void interrupt IntServe(void) {
  if (TMR2IF) {
    TIME = TIME + 1;
    DIR = 1;

    // 128 = 256 / 2 - 1
    // 0 rev/s was easily obtained when the deadzone was +/- 8
    // 1 rev/s corresponds to a delay of 5 ms so we add 5 to SPEED, 
    // so when (PORTB) or (255-PORTB) = 0 the speed is 1 rev/s
    if (PORTB > 135) {
      DIR = 1;
      SPEED = 255 - PORTB + 5;
    } else if (PORTB < 121) {
      DIR = -1;
      SPEED = PORTB + 5;
    } else {
      SPEED = 0;
    }

    N = (N + 1) % SPEED;
    if (N == 0) {
      STEP = STEP + DIR;
      if (STEP > 3) STEP = 0;
      if (STEP < 0) STEP = 3; 
      PORTC = TABLE[STEP];
    }
    TMR2IF = 0;
  }
}

unsigned int a2d_read(void) {
  unsigned int result;
  ADCON0 = 0x01; // Select channel 0, turn on, CLK/32
  GODONE = 1;    // Start conversions
  while(GODONE); // wait until done (approx 8us)
  return(ADRES); // and return the result
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

void string_write_number(long number, char size, char decimal_index, char* string, char index) {
  char output[5];
  
  for (int i=0; i<size; i++) {
    output[i] = number % 10;
    number    = number / 10;
  }

  for (int i=size; i>0; i--) {
    if (i == decimal_index) {
      string[index++] = '.';
    }
    string[index++] = output[i-1] + '0';
  }
}

void initialize(void) {
  TRISA  = 0xFF;
  TRISB  = 0;
  TRISC  = 0;
  TRISD  = 0;
  TRISE  = 0;
  ADCON0 = 0x01;
  ADCON1 = 0x07;
  ADCON2 = 0x85;

  STEP = 0;
  DIR = 1;
  SPEED = 100;
  
  LCD_Init();
}

void main(void) {
  initialize();
  
  // Send title to LCD
  write_string("hw07.c", 0);

  // Setup Interrupts
  T2CON  = 0x4D; // A = 10 (0b1001) C = 4 (0b01)
  PR2    = 249;  // B = 250 (249)
  TMR2ON = 1;
  TMR2IE = 1;
  PEIE   = 1;
  GIE    = 1;

  char string[] = "s: x.xxx d: x";

  while(1) {
    // Read voltage across potentiometer
    unsigned int result = a2d_read();
    PORTB = result >> 2; // Dividing by 4

    // Output speed and direction
    if (SPEED == 0) string_write_number(SPEED,                                 4, 3, string, 3);
    else            string_write_number(1000 - (long)(SPEED - 5) * 1000 / 119, 4, 3, string, 3);  
    if (DIR > 0)    string[12] = '+';
    else            string[12] = '-';
    write_string(string,1);
  }
}      
