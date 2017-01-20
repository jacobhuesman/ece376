; --- program CountABCD.asm ----
;
; 32-bit counter displayed on port A:B:C:D

; -- Variables --
_D EQU 0
_C EQU 1
_B EQU 2
_A EQU 3

#include <p18f4620.inc>
	org 0x800
Main:
	clrf TRISA
	clrf TRISB
	clrf TRISC
	clrf TRISD
	clrf TRISE
	clrf _A
	clrf _B
	clrf _C
	clrf _D
	movwf 0x0F
	movwf ADCON1
Loop:
	incf _D,F
	btfss STATUS,Z
	goto Display
	incf _C,F
	btfss STATUS,Z
	goto Display
	incf _B,F
	btfss STATUS,Z
	goto Display
	incf _A,F

Display:
	movf   _D,W
	movwf  PORTD
	movf   _C,W
	movwf  PORTC
	movf   _B,W
	movwf  PORTB
	movf   _A,W
	movwf  PORTA

	goto Loop
	end