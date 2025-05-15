f_delay:
	; pequeno delay para o efeito de pov
	ldi r19,255
	ldi r26,30
loop_delay:
	; loop do delay
	dec r19
	brne loop_delay 
	dec r26
	brne loop_delay 
	ret