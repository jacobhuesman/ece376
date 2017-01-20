#include <p18f4620.inc>
    org 0x800
    
; -- Variables --   
; unsigned char A, B, C
A0 equ 0
B0 equ 1
C0 equ 2

; A = 5; 
 movlw 5
 movwf A0

; B = 6; 
 movlw 6
 movwf B0

; C = A + B; 
 movf B0,W
 addwf A0,W
 movwf C0
 
 END