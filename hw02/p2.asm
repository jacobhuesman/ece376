#include <p18f4620.inc>
    org 0x800
    
; -- Variables --
;unsigned int A, B, C;
A0 equ 0
A1 equ 1
B0 equ 3
B1 equ 4
C0 equ 5
C1 equ 6 
    
;A = 500 = 0x01F4;
 movlw 0xF4
 movwf A0
 movlw 0x01
 movwf A1
    

;B = 600 = 0x0258
 movlw 0x58
 movwf A0
 movlw 0x02
 movwf A1
    

;C = A + B;
 movf A0,W
 addwf B0,W
 movwf C0

 movf A1,W
 addwfc B1,W
 movwf C1
 
 end
 