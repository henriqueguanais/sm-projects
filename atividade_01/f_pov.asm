f_pov:
	; responsavel por fazer o efeito de pov, alternando os transistores
	inc cont_cont_pov

    cpi cont_pov, 3			; mostra centena
    breq ligar_tj3
    cpi cont_pov, 2			; mostra dezena
    breq ligar_tj2
    cpi cont_pov, 1			; mostra unidade
    breq ligar_tj1

reset_cont_pov:
    ldi cont_pov, 3
	rjmp f_pov

ligar_tj3:
    ; centena
	cbi PORTD, 2			; bit porta D2 LOW
	cbi PORTD, 3			; bit porta D3 LOW
	cpi r16,100				; se o numero atual for maior que 100, liga o display de centena
	brsh liga_centena
    cbi PORTB, 0			; bit porta B0 HIGH
	ret

ligar_tj2:
    ; dezena
    cbi PORTB, 0			; bit porta B0 LOW
	cbi PORTD, 3			; bit porta D3 LOW
	sbi PORTD, 2			; bit porta D2 HIGH
	ret

ligar_tj1:
    ; unidade
    cbi PORTB, 0			; bit porta B0 LOW
	cbi PORTD, 2			; bit porta D2 LOW
	cpi r16,10				; se o numero atual for maior que 10, liga o display de dezena
	brsh liga_dezena
	cbi PORTD, 3			; bit porta D3 HIGH
	ret

liga_centena:
	sbi PORTB, 0 
	ret

liga_dezena:
	sbi PORTD, 3
	ret

