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
RUN equ 2
CURRENT_WAIT equ 3
;SEC equ 3
;SEC100 equ 4

; --- Main Routine ---
Main:
  call Init
  call WaitToRun
 
; --- Subroutines ---
Init:
  clrf I0
  clrf I1
  clrf TRISC
  clrf TRISD
  clrf SEC
  clrf SEC100
  clrf RUN
  clrf CURRENT_WAIT
  movlw 1  ; Setting to 1 until button implementation is finished
  movwf RUN
  return

WaitToRun:
  movlw 1
  cpfseq RUN
    goto WaitToRun
  call WhileLoop

WhileLoop:
  clrf SEC100
  call ForLoop
  incf SEC,F
  goto WhileLoop

ForLoop:
  movlw 100
  cpfslt SEC100
	  return
  movlw 0
  movwf CURRENT_WAIT
  call Wait
  incf SEC100,F
  goto ForLoop
  

;; Using recursive strategy
;; Each time Wait is called it calls another instance of itself.
Wait:
  
  incf CURRENT_WAIT
  movlw 1
  cpfslt CURRENT_WAIT
    call Wait
  
  movlw 100
  cpfslt I1
    return
  incf I1,F
  decf CURRENT_WAIT
  goto Wait
    



 end
    
    
    


