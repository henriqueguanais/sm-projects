HCSR04:
	call delay_10us	
	ldi r16, 0x10			
	out PORTC, r16					; ativa o trigger do sensor
	call delay_10us					
	call delay_10us					; segura por 20us ao todo

	clr r16
	out PORTC, r16					; desativa o trigger do sensor

	call delay_10us
	call delay_10us

ler_echo:
	clr r24

	in r24, PINC					; le o pino do echo
	andi r24,0x20
	cpi r24, 0x20					; verifica se o pino do echo esta alto
	breq calc_distancia

	rjmp ler_echo

calc_distancia:
	in r24, PINC
	andi r24,0x20
	cpi r24, 0x00					; verifica se o pino do echo esta baixo
	breq fim_calc
	adiw Z, 1

	rjmp calc_distancia

fim_calc:
	ret

delay_10us:
    ldi r19, 40
delay_loop10us:
    dec r19
    brne delay_loop10us
    ret
