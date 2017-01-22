;; --- Homework #2 Part 3 ---
;;
;; Convert the following C code to assembler:
;;
;; unsigned char A, i;
;; 
;; while(1) {
;;   A = A + 1; 
;;   for (i=0; i<10; i++);
;; } 
;;
  
; --- Configuration ---
#include <p18f4620.inc>
 org 0x800
    
; unsigned char A, i;
#define A0 PORTC
#define I0 PORTD

; --- Main Routine ---
Main:
  call Init
  call WhileLoop
 
; --- Subroutines ---
Init:
  clrf TRISC
  clrf TRISD

WhileLoop:  ; while(1) {
  ; A = A + 1; 
  incf A0,F
  
  ; for (i=0; i<10; i++);
  clrf I0
  call ForLoop
  
  ; }
  goto WhileLoop

ForLoop:
  movlw 10
  cpfslt I0
	  return

  incf I0,F

  goto ForLoop
  
Shutdown: 
 end
