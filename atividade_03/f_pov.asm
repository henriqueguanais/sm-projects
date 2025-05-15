f_pov:
	inc cont_cont_pov
    cpi cont_pov, 3
    breq ligar_tj3
    cpi cont_pov, 2
    breq ligar_tj2
    cpi cont_pov, 1
    breq ligar_tj1

reset_cont_pov:
    ldi cont_pov, 3
	rjmp f_pov

ligar_tj3:
    ; centena
	cbi PORTD, 2; bit porta D2 LOW
	cbi PORTD, 3; bit porta D3 LOW
    sbi PORTB, 0; bit porta B0 HIGH
	ret

ligar_tj2:
    ; unidade
    cbi PORTB, 0; bit porta B0 LOW
	cbi PORTD, 3; bit porta D3 LOW
	sbi PORTD, 2; bit porta D2 HIGH
	ret

ligar_tj1:
    ; dezena
    cbi PORTB, 0; bit porta B0 LOW
	cbi PORTD, 2; bit porta D2 LOW
	sbi PORTD, 3; bit porta D3 HIGH
	ret

