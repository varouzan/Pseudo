$NOMOD51


$include (SI_EFM8BB3_Defs.inc)            ; Include register definition file.

LEDG   equ   P1.4
LEDB   equ	 P1.5
LEDR   equ   P1.6
                    ; Green LED: '1' is ON
                   ; Display Enable:
                                           ;   '0' is Board Contoller driven
                                           ;   '1' is EFM8 driven
;-----------------------------------------------------------------------------
; RESET and INTERRUPT VECTORS
;-----------------------------------------------------------------------------

            ; Reset Vector
        ;    cseg AT 0
             jmp Main                     ; Locate a jump to the start of
                                          ; code at the reset vector.

;-----------------------------------------------------------------------------
; CODE SEGMENT
;-----------------------------------------------------------------------------



SET1:mov R6, #60h
SET2:mov R7, #60h
SET3:mov R5, #60h
DELAY: NOP
djnz  R5, DELAY
djnz  R7, SET3
djnz  R6, SET2
RET


Main:
            ; Disable the WDT.
            anl   PCA0MD, #NOT(040h)      ; clear Watchdog Enable bit

            ; Enable the Port I/O Crossbar
            orl   P1SKIP, #02h            ; skip LED pin in crossbar
                                          ; assignments
            mov   XBR2, #40h
            orl   P1MDOUT, #02h           ; make LED pin output push-pull
			mov   R4, #03h

		CLR  LEDB
		CLR  LEDR
		CLR  LEDG
		LCALL SET1
		SETB LEDR
		SETB LEDB
		CLR  LEDG
		LCALL SET1
		CPL LEDB
		CPL LEDG
		LCALL SET1
		CPL LEDR
		CPL LEDB
		LCALL SET1



END

