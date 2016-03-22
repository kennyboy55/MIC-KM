;
; MIC-Week6.asm
;
; Created: 22-3-2016 11:18:06
; Author : Kenneth en Martijn
;

; demoprogramma 6_B: 16 bits optelling
;
.INCLUDE "m128def.inc"
;
.CSEG
.ORG 	$0000
		rjmp main
;
.CSEG
.ORG	$0200
;
main:						;
; 16 bits optelling: r5/r4 = r9/r8 + r7/r6  (23 ab + 44 98 = 68 43)
					; Stap 1: 	tel de LSB?s op: r4 = r6+r8 
	mov	r4, r8		; 	r4 <- r8 		
	add	r4, r6		; 	r4 <- r4+r6 		 misschien Carry gezet 
					; Stap 2: 	tel de MSB?s op met 
					;			de (eventuele) carry uit LSB?s:
					;			r3 = r5+r6+c
	mov	r5, r9		; r5 <- r9 
	adc	r5, r7		; r5 <- r5+r7+c 

		
no_end:   			; unlimited loop, when done
		rjmp no_end	