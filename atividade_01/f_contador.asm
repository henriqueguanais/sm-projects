f_contador:
    cpi cont_pov, 3
    breq mostrar_unidade
    cpi cont_pov, 2
    breq mostrar_dezena
    cpi cont_pov, 1
    breq mostrar_centena

mostrar_unidade:
	mov r29,valor_unidade
	rjmp comparacao

mostrar_dezena:
	mov r29,valor_dezena
	rjmp comparacao

mostrar_centena:
	mov r29,valor_centena
	rjmp comparacao

comparacao:	
	cpi r29,0
	breq case_0
	cpi r29,1
	breq case_1
	cpi r29,2
	breq case_2
	cpi r29,3
	breq case_3
	cpi r29,4
	breq case_4
	cpi r29,5
	breq case_5
	cpi r29,6
	breq case_6
	cpi r29,7
	breq case_7
	cpi r29,8
	breq case_8
	cpi r29,9
	breq case_9
	jmp default

case_0:
	ldi r28, 0x00
	out PORTD,r28
	dec cont_pov
    ret
case_1:
	ldi r28, 0x80
	out PORTD,r28
	dec cont_pov
    ret
case_2:
	ldi r28, 0x40
	out PORTD,r28
	dec cont_pov
    ret
case_3:
	ldi r28, 0xc0
	out PORTD,r28
	dec cont_pov
    ret
case_4:
	ldi r28, 0x20
	out PORTD,r28
	dec cont_pov
    ret
case_5:
	ldi r28, 0xa0
	out PORTD,r28
	dec cont_pov
    ret
case_6:
	ldi r28, 0x60
	out PORTD,r28
	dec cont_pov
    ret
case_7:
	ldi r28, 0xe0
	out PORTD,r28
	dec cont_pov
    ret
case_8:
	ldi r28, 0x10
	out PORTD,r28
	dec cont_pov
    ret
case_9:
	ldi r28, 0x90
	out PORTD,r28
	dec cont_pov
    ret
default:
	ldi r28, 0x00
	out PORTD,r28
	dec cont_pov
    ret