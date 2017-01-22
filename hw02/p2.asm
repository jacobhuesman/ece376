;; --- Homework #2 Part 2 ---
;;
;; Convert the following C code to assembler:
;; 
;; unsigned int A, B, C;
;;
;; A = 500;
;; B = 600;
;; C = A + B;
;;

; --- Configuration ---
#include <p18f4620.inc>
    org 0x800

; unsigned int A, B, C;
A0 equ 0
A1 equ 1
B0 equ 3
B1 equ 4
#define C0 PORTD
#define C1 PORTC
    

; --- Main Routine ---
Main:
  call Init
  call Run


; --- Subroutines ---
Init:
  clrf TRISC
  clrf TRISD
  
Run:
  ; A = 500 = 0x01F4;
  movlw 0xF4
  movwf A0
  movlw 0x01
  movwf A1
    
  ; B = 600 = 0x0258
  movlw 0x58
  movwf B0
  movlw 0x02
  movwf B1
    
  ; C = A + B = 0x044C;
  movf A0,W
  addwf B0,W
  movwf C0
  movf A1,W
  addwfc B1,W
  movwf C1
  
  goto Run
  
Shutdown: 
  end
