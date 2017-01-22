;; --- Homework #2 Part 1 ---
;;
;; Convert the following C code to assembler:
;; 
;; unsigned char A, B, C;
;;
;; A = 5;
;; B = 6;
;; C = A + B;
;;


; --- Configuration ---
#include <p18f4620.inc>
    org 0x800

; unsigned char A, B, C
#define A0 PORTB
#define B0 PORTC
#define C0 PORTD

    
; --- Main Routine ---
Main:
  call Init
  call Run


; --- Subroutines ---
Init:
  clrf TRISB
  clrf TRISC
  clrf TRISD

Run:
  ; A = 5; 
  movlw 5
  movwf A0

  ; B = 6; 
  movlw 6
  movwf B0

  ; C = A + B; 
  movf B0,W
  addwf A0,W
  movwf C0
  goto Run
 
Shutdown:
  end
