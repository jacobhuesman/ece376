;; --- Homework #2 Part 4 ---
;;
;; Convert the following C code to assembler:
;;
;; unsigned char A, B, C;
;; 
;; if (A) C = C + 1;
;; if (B) C = C - 1;
;;  
  
; --- Configuration ---
#include <p18f4620.inc>
 org 0x800
    
; unsigned char A, i;
#define A0 PORTB
#define B0 PORTC
#define C0 PORTD
TRUE equ 1
FALSE equ 0

; --- Main Routine ---
Main:
  call Init
  call Run
  call Spin
 
; --- Subroutines ---
Init:
  clrf TRISB
  clrf TRISC
  clrf TRISD
  clrf A0
  clrf B0
  clrf C0

  movlw TRUE
  movwf B0
  
  return
  
Run:
  If0:
    movlw TRUE
    cpfseq A0
	    goto EndIf0
    incf C0,F
  EndIf0:

  If1:
    movlw TRUE
    cpfseq B0
      goto EndIf1
    decf C0,F
  EndIf1

  return

Spin:
  goto Spin
  
Shutdown: 
 end
