HCSR04:
	call delay_10us
	ldi r16, 0x10
	out PORTC, r16
	call delay_10us
	call delay_10us
	clr r16
	out PORTC, r16

	call delay_10us
	call delay_10us


ler_echo:

rjmp calc_distancia
	clr r24
	in r24, PINC
	andi r24,0x20
	cpi r24, 0x20
	breq calc_distancia
	;mov ZL, r24
	;call f_valor     	; calcula a quantidade de centena, dezena e unidade a ser exibida
    ;call f_show_cont 	; exibe contador
    ;call f_pov       	; faz o efeito pov
	;call f_delay     	; delay para o contador

	rjmp ler_echo

calc_distancia:
	in r24, PINC
	andi r24,0x20
	cpi r24, 0x00
	;breq fim_calc
	;cpi r16, 50
	;brsh inc_cm
	adiw Z, 1
	inc r16
	rjmp calc_distancia

inc_cm:
	rjmp calc_distancia

fim_calc:
	ret

delay_10us:
    ldi r19, 40
delay_loop10us:
    dec r19
    brne delay_loop10us
    ret
