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
#include <p18f4620.inc>
 org 0x800
    
#define SEC PORTC
#define SEC100 PORTD
#define I INDF0
#define IL FSR0L

I0 equ 0
I1 equ 1
I2 equ 2
I3 equ 3
I4 equ 4
RUN equ 5


; --- Main Routine ---
Main:
  call Init
  call WaitToRun
  call Counter
 

; --- Subroutines ---
Init:
  clrf I0
  clrf I1
  clrf I2
  clrf I3
  clrf I4
  clrf TRISB
  clrf TRISC
  clrf TRISD
  clrf FSR0L
  clrf FSR0H
  clrf INDF0
  clrf SEC
  clrf SEC100
  clrf RUN
  movlw 1  ; Setting to 1 until button implementation is finished
  movwf RUN
  return

WaitToRun:
  movlw 1
  cpfseq RUN
    goto WaitToRun
  return

Counter:
  clrf SEC100
  clrf IL
  clrf I0
  call CounterMs
  incf SEC,F
  goto Counter


;; Using recursive strategy
CounterMs:
  movf I0,W
  movwf SEC100
  incf IL

  movlw 3
  cpfseq IL
    call CounterMs
  clrf I
  
  decf IL,F

  incf I,F
  
  movlw 100
  cpfslt I
    return

  goto CounterMs


 end
    
    
    


