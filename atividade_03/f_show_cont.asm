f_show_cont:
	clr r20
    cpi cont_pov, 3
    breq mostrar_centena
    cpi cont_pov, 2
    breq mostrar_dezena
    cpi cont_pov, 1
    breq mostrar_unidade

mostrar_unidade:
	mov r20,valor_unidade
	rjmp comparacao

mostrar_dezena:
	mov r20,valor_dezena
	rjmp comparacao

mostrar_centena:
	mov r20,valor_centena
	rjmp comparacao

comparacao:	
	cpi r20,0
	breq case_0
	cpi r20,1
	breq case_1
	cpi r20,2
	breq case_2
	cpi r20,3
	breq case_3
	cpi r20,4
	breq case_4
	cpi r20,5
	breq case_5
	cpi r20,6
	breq case_6
	cpi r20,7
	breq case_7
	cpi r20,8
	breq case_8
	cpi r20,9
	breq case_9
	jmp default

case_0:
	ldi r20, 0x00
	out PORTD,r20
	dec cont_pov
    ret
case_1:
	ldi r20, 0x80
	out PORTD,r20
	dec cont_pov
    ret
case_2:
	ldi r20, 0x40
	out PORTD,r20
	dec cont_pov
    ret
case_3:
	ldi r20, 0xc0
	out PORTD,r20
	dec cont_pov
    ret
case_4:
	ldi r20, 0x20
	out PORTD,r20
	dec cont_pov
    ret
case_5:
	ldi r20, 0xa0
	out PORTD,r20
	dec cont_pov
    ret
case_6:
	ldi r20, 0x60
	out PORTD,r20
	dec cont_pov
    ret
case_7:
	ldi r20, 0xe0
	out PORTD,r20
	dec cont_pov
    ret
case_8:
	ldi r20, 0x10
	out PORTD,r20
	dec cont_pov
    ret
case_9:
	ldi r20, 0x90
	out PORTD,r20
	dec cont_pov
    ret
default:
	ldi r20, 0x00
	out PORTD,r20
	dec cont_pov
    ret