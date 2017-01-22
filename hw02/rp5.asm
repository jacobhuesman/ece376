;; --- Homework #2 Part 5 ---
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
#define SEC100 PORTD
#define I INDF0
#define IL FSR0L
#define LCnts INDF1
#define LCntsI FSR1L

TRUE equ 1
FALSE equ 0

I0 equ 0
I1 equ 1
I2 equ 2
I3 equ 3
LCnts0 equ 4
LCnts1 equ 5
LCnts2 equ 6
LCnts3 equ 7
RUN equ 8


; --- Main Routine ---
Main:
  call Init
  call WaitToRun
  call Counter
  goto Main
 

; --- Subroutines ---
Init:
  clrf TRISB
  clrf TRISC
  clrf TRISD

  ; 4x1 Array I
  clrf I0
  clrf I1
  clrf I2
  clrf I3

  ; 4x1 Array LCnts
  ; Equation to determine coefficients:
  ; 1 = 2u * (1 + 100(1 + L3(1 + L4)))
  ; Current coefficient selection leads to an 
  ; error of .05 seconds every 256 seconds
  movlw 0
  movwf LCnts0
  movlw 100
  movwf LCnts1
  movlw 238
  movwf LCnts2
  movlw 20
  movwf LCnts3
  
  clrf SEC
  clrf SEC100
  clrf RUN

  ; Set pointer 0 to address 0x000-0x003
  clrf FSR0L
  clrf FSR0H
  clrf INDF0
  
  ; Set ponter 1 to address 0x004-0x007 
  clrf FSR1L
  clrf FSR1H
  movlw LCnts0
  movwf LCntsI

  movlw TRUE  ; Setting to 1 until button implementation is finished
  movwf RUN

  return

WaitToRun:
  movlw TRUE
  cpfseq RUN
    goto WaitToRun
  return

Counter:
  ; --- Overall clock cycles: 18 ---
  ; Uses recursion

  ; Display the value of I0 on PORTD
  ; Clock cycles: 4
  movf I0,W
  movwf SEC
  movf I1,W
  movwf SEC100
  
  ; If the current level is less than 2, call CounterMs
  ; Clock cycles: 10
  incf IL
  incf LCntsI
  
  If0:
    movlw 4  ; Current level + 1
    cpfslt IL
      goto EndIf0
    call Counter
  EndIf0:
  
  clrf I
  decf IL,F
  decf LCntsI
  incf I,F

  ; Burn some cycles
  ; Clock cycles: 2
  nop
  nop
  
  ; If current level has been iterated over x # of times, return
  ; Clock cycles: 4
  If1:
    movf LCnts,W
    cpfseq I
      goto Counter  ; Else
    return
  EndIf1:

Shutdown:
  end
