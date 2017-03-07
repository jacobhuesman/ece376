
// Global Variables
unsigned char TABLE[4] = {1, 2, 4, 8};
int DIR, SPEED, STEP, N, TIME;


// Subroutine Declarations
#include <pic18.h>

// High-priority service. 
// Called every 1ms. 
// 200 steps per rotation
void interrupt IntServe(void) {
  if (TMR2IF) {
    TIME = TIME + 1;
    DIR = 1;

    // 256 slowest speed
    // 12 fastest speed
    if (PORTB > 136) {
      DIR = 1;
      SPEED = 255 - PORTB + 5;
    } else if (PORTB < 120) {
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
  GODONE = 1; // Start conversions
  while(GODONE); // wait until done (approx 8us)
  return(ADRES); // and return the result
}

// Main Routine

void main(void) {
  int i;

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


  // initialize Timer2
  T2CON = 0x4D; // A = 10 (0b1001) C = 4 (0b01)
  PR2 = 249;    // B = 250 (249)
  TMR2ON = 1;
  TMR2IE = 1;
  PEIE = 1;
  GIE = 1;

  while(1) {
    unsigned int result = a2d_read();
    PORTB = result / 4;
  }
}      
