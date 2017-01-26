;; --- Homework #3 Part 6 ---
;;
;; Design an embedded system which uses binary inputs and outputs and timing.
;;
;; Write an assembler program which corresponds to a stopwatch
;;
;; RUN = 1: Start the stopwatch
;; Sec100   8-bit number. 1/100 counter
;; Sec      8-bit number. seconds
;;    
;; --- Pseudo Code ---
;; // Declare variables
;; unsigned char RUN, Sec100, Sec;
;;
;; // Initialize
;; RUN = 0;
;; Sec100 = 0;
;; Sec = 0;
;;
;; // Start stopwatch
;; while(RUN != 1);
;; 
;; // Count seconds
;; while(1){
;;   for (i=0; i<100; i++){
;;     wait_however_many_tics
;;   }
;;   Sec++;
;; }
;; 
;; Notes:
;; One clock cycle = 100ns
;; Each instruction takes 1 clock cycle to complete
;;
  

; --- Configuration ---

  errorlevel -207  ; Turn off label identation warnings
  org 0x800

#include <p18f4620.inc>
    
#define SEC PORTC
#define CSEC PORTD

TRUE equ 1
FALSE equ 0

C0 equ 0
C1 equ 1
RUN equ 8


; --- Main Routine ---
Main:
  call Init
  call WaitToRun
  call Counter
  goto Main
 

; --- Subroutines ---
Init:
  clrf TRISC
  clrf TRISD

  clrf C0
  clrf C1
  clrf SEC
  clrf CSEC
  clrf RUN

  movlw TRUE
  movwf RUN

  return

WaitToRun:
  movlw TRUE
  cpfseq RUN
    goto WaitToRun
  return

Counter:
  ; Clock cycles: 99994 = 17 * 5882
  call Wait1cs
  
  ; Clock cycles: 6
  If0:
    movlw 99
    cpfseq CSEC
      goto EndIf0
    clrf CSEC
    incf SEC,F
  EndIf0:
  incf CSEC

  goto Counter
    
Wait1cs:
  ; Check to see if we've incremented X # of times.
  ; Clock Cycles 17
  ; Loops: 5882 = 0x16FA
  If1:
    movlw 0x16
    cpfseq C1
      goto EndIf1
    If2:
      movlw 0xFA
      cpfseq C0
        goto EndIf2
      clrf C0
      clrf C1
      call Wait6CC
      nop  ; 1 nop plus the first 2 clock cycles needed to call Wait1cs
      return
  EndIf1:
    goto EndIf2
  EndIf2:

  ; Increment low byte and add carry
  incf C0
  movlw 0
  addwfc C1,1

  call Wait6CC

  goto Wait1cs

Wait6CC:
  nop
  nop
  return

Shutdown:
  end
