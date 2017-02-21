/* 
 * Assignment 4 Part 1
 * Jacob Huesman, Nick Carlson
 */


#include <pic18.h>

   
void main(void){
  TRISC = 0;
  unsigned char sec = 0;

  while(1) {
    sec = sec + 1;
    PORTC = sec;
  }
}
