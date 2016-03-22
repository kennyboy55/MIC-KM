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
main:	
; Opgave 0					;
/*; 16 bits optelling: r5/r4 = r9/r8 + r7/r6  (23 ab + 44 98 = 68 43)
					; Stap 1: 	tel de LSB?s op: r4 = r6+r8 
	mov	r4, r8		; 	r4 <- r8 		
	add	r4, r6		; 	r4 <- r4+r6 		 misschien Carry gezet 
					; Stap 2: 	tel de MSB?s op met 
					;			de (eventuele) carry uit LSB?s:
					;			r3 = r5+r6+c
	mov	r5, r9		; r5 <- r9 
	adc	r5, r7		; r5 <- r5+r7+c 
					;
					;*/
					;
; Opgave 1
/*	ldi r16, $55	;
	sts $540, r16	;
	sts $541, r16	;
	sts $542, r16	;
	sts $543, r16	;
	sts $544, r16	;
	sts $545, r16	;
	sts $546, r16	;
	sts $547, r16	;
	sts $548, r16	;

	ld r17*/
					;
;Opgave 2 poging 1

/*	ldi r16, $01	;
	ldi r17, $60	;
	ldi r18, $70	;

loop:
	lds r4, r17		;
	com r4			;
	sts r17, r4		;
	add r17, r16	;
	cp r17, r18		;
	breq no_end		;
	rjmp loop		;*/
	ldi r16, $01	;
	lds r17, $60	;
	com r17	
	sts $60, r17	;
	lds r17, $61	;
	com r17			;
	lds r17, $62	;
	com r17			;
	lds r17, $63	;
	com r17			;
	lds r17, $64	;
	com r17			;
	lds r17, $65	;
	com r17			;
	lds r17, $66	;
	com r17			;
	lds r17, $67	;
	com r17			;
	lds r17, $68	;
	com r17			;
	lds r17, $69	;
	com r17			;
	lds r17, $70	;
	com r17			;

		
no_end:   			; unlimited loop, when done
		rjmp no_end	