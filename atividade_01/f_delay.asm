f_delay:
	ldi r19,255
	ldi r26,30
; loop do delay
loop_delay:
	dec r19
	brne loop_delay 
	dec r26
	brne loop_delay 
	ret