;; --- p5.asm ---
;; This program solves the following problem:
;;
;; Write an assembler program which corresponds to a stopwatch
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
  
;; --- Configuration ---
#include <p18f4620.inc>
 org 0x800
    
#define SEC PORTC
#define SEC100 PORTD

; -- Variables --
I0 equ 0
I1 equ 1
I2 equ 2
RUN equ 3
CURRENT_WAIT equ 4
;SEC equ 3
;SEC100 equ 4

; --- Main Routine ---
Main:
  call Init
  call Loop
 
; --- Subroutines ---
Init:
  clrf I0
  clrf I1
  clrf TRISC
  clrf TRISD
  clrf FSR0L
  clrf FSR0H
  clrf INDF0
  movlw 5
  movwf I0
  movlw 2
  movwf INDF0
  return
  
Loop:
  movf I0,W
  movwf PORTD
  movf INDF0,W
  movwf PORTC
  goto Loop
  


 end
    
    
    


