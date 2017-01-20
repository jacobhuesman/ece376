;; --- Pseudo Code ---
;; while(1) {
;;   A = A + 1; 
;;   for (i=0; i<10; i++);
;; } 
  
;; --- Implementation ---
#include <p18f4620.inc>
 org 0x800
    
; -- Variables --
;; unsigned char A, i;
A equ 0
I equ 1

; --- Main Routine ---
Main:
    call WhileLoop
 
; --- Subroutines ---
;; while(1) {
WhileLoop:
  ;; A = A + 1; 
  incf A,F
  ;; for (i=0; i<10; i++);
  clrf I
  goto ForLoop
  ;; }
  goto WhileLoop

ForLoop:
    movlw 10
    cpfslt I
	return
    INCF I,F
    goto ForLoop
  
    
 end
    
    
    
