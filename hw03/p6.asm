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
  movlw 0xFF
  movwf TRISB ;PORTB is input
  clrf TRISC  ;PORTC is output
  clrf TRISD  ;PORTD is output

  call Clear

  movlw 15
  movwf ADCON1  ;everyone is binary

  movlw TRUE
  movwf RUN

  return

Clear:
  clrf C0
  clrf C1
  clrf SEC
  clrf CSEC
  clrf RUN
  return

WaitToRun:
  btfsc PORTB,2
    call Clear
  btfsc PORTB,0
    return
  goto WaitToRun

Counter:
  ; Clock cycles: 99992 = 29 * 3448
  call Wait1cs
  ; Clock cycles: 8
  btfsc PORTB,1
    call WaitToRun
  If0:
    movlw 100
    cpfseq CSEC
      goto EndIf0
    clrf CSEC
    incf SEC,F
  EndIf0:
  incf CSEC

  goto Counter
    
Wait1cs:
  ; Check to see if we've incremented X # of times.
  ; Clock Cycles 17 need 29
  ; Loops: 3448 = 0x0D78
  If1:
    movlw 0x0D
    cpfseq C1
      goto EndIf1
    If2:
      movlw 0x78
      cpfseq C0
        goto EndIf2
      clrf C0
      clrf C1
      call Wait18CC
      nop  ; 1 nop plus the first 2 clock cycles needed to call Wait1cs
      return
  EndIf1:
    goto EndIf2
  EndIf2:

  ; Increment low byte and add carry
  incf C0
  movlw 0
  addwfc C1,1

  call Wait18CC

  goto Wait1cs

Wait7CC:
  nop
  nop
  nop
  return

Wait18CC
  call Wait7CC
  call Wait7CC
  return

Shutdown:
  end
