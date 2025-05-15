PCINT1_ISR:
	clr r24
	in r24, PINC
	andi r24,0x0e
	cpi r24,0x06						
	breq case_exibe_memoria			    ; S3 pressionado
	cpi r24,0x0a
	breq case_salva_memoria			    ; S2 pressionado
	cpi r24,0x0c
	breq case_mede_distancia			; S1 pressionado
	reti

case_exibe_memoria:
	ldi estado, 0x03
    reti

case_salva_memoria	:
	ldi estado, 0x02
    reti

case_mede_distancia:
	ldi estado, 0x01
    reti

