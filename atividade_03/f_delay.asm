f_delay:
	ldi r16,0xff
	ldi r20,0x1e
; loop do delay
loop_delay:
	dec r19
	brne loop_delay 
	dec r20
	brne loop_delay 
	ret
