.include "m328pdef.inc"

.def cont_pov = r18
.def cont_cont_pov = r21
.def valor_centena = r0
.def valor_dezena = r1
.def valor_unidade = r2
.def valor_passo = r3
.def estado = r17

.cseg
.org 0x0000
    rjmp main
.org 0x008 					; interrupcao do PCINT1 
	jmp PCINT1_ISR

main:
	ldi estado, 0x00    	; estado inicial
	ldi r16, (0xF0 & DDRC)
	out DDRC, r16	  		; define os pinos PC3 a PC0 como entrada, btn e potenciometro

	ldi r16,0x02 			; define interrupcao botao
	sts PCICR, r16
	ldi r16,0x0E
	sts PCMSK1,r16
	sei

	; configura leitura ADC
	ldi r16, 0x60
	sts ADMUX, r16   		; defina AVcc como referencia e ajuste a esquerda             
	ldi r16, 0x00
	sts ADCSRB, r16  		; defina o modo de trigger free running		
	ldi r16, 0x87    
	sts ADCSRA, r16  		; habilite o adc e utilize uma pre-escala de 128
	
	ldi r20, 0xfc
    out DDRD, r20  			; define os pinos PD7-PD2 como saida
	sbi DDRB,0      		; define PB0 como saida

    ldi cont_pov, 3    		; contador de 3 elementos para o efeito pov
	clr cont_cont_pov  		; contador da quantidade de pov 

	ldi r16 , 0x01			; valor do passo, fabrica = 1
	mov valor_passo, r16

	ldi XL, low(999) 		; valor_min_L
	ldi XH, high(999) 		; valor_min_H
	clr XL            		; valor de fabrica = 0
	clr XH

	ldi YL, low(999) 		; valor_max_L
	ldi YH, high(999) 		; valor_max_H
	ldi YL, 0xe7       		; valor de fabrica = 999
	ldi YH, 0x03

	ldi ZL, low(999) 		; valor_atual_L
	ldi ZH, high(999) 		; valor_atual_H
	mov ZH, XH        		; comeca em 0
	mov ZL, XL 

	call f_RGB      		; inicializa o RGB

loop: 
	call f_contador 		; valor atual do contador
	call f_valor     		; calcula a quantidade de centena, dezena e unidade a ser exibida
    call f_show_cont 		; exibe contador
    call f_pov       		; faz o efeito pov
	call f_mde	 			; maquina de estados 
	call f_delay     		; delay para o contador

    rjmp loop

; =======================================================
; 						f_pov
; =======================================================
f_pov:
	inc cont_cont_pov

    cpi cont_pov, 3			; mostra centena
    breq ligar_tj3
    cpi cont_pov, 2			; mostra dezena
    breq ligar_tj2
    cpi cont_pov, 1			; mostra unidade
    breq ligar_tj1
reset_cont_pov:
	; se cont_pov for 0, reseta o contador
    ldi cont_pov, 3
	rjmp f_pov

ligar_tj3:
    ; centena
	cbi PORTD, 2			; bit porta D2 LOW
	cbi PORTD, 3			; bit porta D3 LOW
    sbi PORTB, 0			; bit porta B0 HIGH
	ret

ligar_tj2:
    ; unidade
    cbi PORTB, 0			; bit porta B0 LOW
	cbi PORTD, 3			; bit porta D3 LOW
	sbi PORTD, 2			; bit porta D2 HIGH
	ret

ligar_tj1:
    ; dezena
    cbi PORTB, 0			; bit porta B0 LOW
	cbi PORTD, 2			; bit porta D2 LOW
	sbi PORTD, 3			; bit porta D3 HIGH
	ret
; =======================================================
; 						f_delay
; =======================================================
f_delay:
	ldi r19,255
	ldi r26,30
loop_delay:
	; loop do delay
	dec r19
	brne loop_delay 
	dec r26
	brne loop_delay 
	ret
; =======================================================
; 						f_contador
; =======================================================
f_contador: 
    cpi cont_cont_pov, 250		; so le o proximo numero se cont_cont_pov for 250
    brsh inc_cont    
    ret							; retorna se o contador ainda nao chegou em 100

inc_cont:
	clr cont_cont_pov
	cpi estado, 0x00			; se o estado for 0, contagem crescente
	breq cont_cres
	cpi estado, 0x01			; se o estado for 1, contagem decrescente
	breq cont_dec
	breq stop_cont				; para qualquer outro estado, para o contador

cont_cres:
	cp ZH,YH   					; compara se o valor de ZH e igual a YH
	breq comp_cres_ZL			; se forem iguais, vai comparar ZL com YL
	add ZL, valor_passo			; incrementa ZL com o valor do passo
	cpi ZL, 0x00			    ; se ZL for 0, entao soma 1 a ZH
	breq add_ZH
	ret

add_ZH:
	inc ZH			
	ret

comp_cres_ZL:
	cp ZL,YL					; se ZL = YL, entao o contador chegou no maximo
	breq stop_cont
	add ZL, valor_passo			; incrementa ZL com o valor do passo
	ret

cont_dec:
	cp ZH,XH					; compara se o valor de ZH e igual a XH
	breq comp_dec_ZL			; se forem iguais, vai comparar ZL com XL
	sbc ZL, valor_passo			; decrementa ZL com o valor do passo
	ret

comp_dec_ZL:
	cp ZL,XL					; se ZL = XL, entao o contador chegou no minimo
	breq stop_cont
	sbc ZL, valor_passo			; decrementa ZL com o valor do passo
	ret

stop_cont:
    nop							; se o contador chegou no maximo, nao faz nada
	ret
; =======================================================
; 						f_valor
; =======================================================
f_valor:
    clr r20
    mov r20,ZL  				; copia ZL para r20

    clr valor_centena   		; contador de centena
    clr valor_dezena   			; contador de dezena
    clr valor_unidade   		; contador de unidade
cent:
	cpi r20,100 				; compara se o numero e igual a 100
	brsh decrementa100  		; se for maior que 100 vai para decrementa100
	rjmp dez

decrementa100:
	subi r20,100 				; subtrai 100 do valor
	inc valor_centena      		; incrementa o contador de centena
    rjmp cent

dez:
	cpi r20,10  				; compara se o numero e igual a 10
	brsh decrementa10  			; se for maior que 10 vai para decrementa10
	rjmp uni

decrementa10:
	subi r20,10  				; subtrai 10 do valor
	inc valor_dezena    		; incrementa o contador de dezena
	cpi r20,10   				; compara com 10 
	rjmp dez  

uni:
	cpi r20,1   				; compara com 1
	brsh decrementa1   			; se for maior que 1 vai para decrementa1
	rjmp comp_ZH

decrementa1:
	subi r20,1  				; subtrai 1 do valor
	inc valor_unidade  			; incrementa o contador de unidade
	rjmp uni   
    
comp_ZH:
	cpi ZH,0x01					; analisa o valor de ZH
	breq case_ZH1
	cpi ZH,0x02
	breq case_ZH2
	cpi ZH,0x03
	breq case_ZH3
	ret

case_ZH1:
	ldi r20,0x02				; se ZH for 00000001, entao adiciona 2 as centenas 5 dezena e 6 unidade
	add valor_centena, r20
	ldi r20,0x05
	add valor_dezena, r20
	ldi r20,0x06			
	add valor_unidade, r20 
	rjmp verifica_carry

case_ZH2:
	ldi r20,0x05				; se ZH for 00000010, entao adiciona 5 as centenas 1 dezena e 2 unidade
	add valor_centena, r20
	ldi r20,0x01
	add valor_dezena, r20
	ldi r20,0x02
	add valor_unidade, r20
	jmp verifica_carry
	
case_ZH3:
	ldi r20,0x07				; se ZH for 00000011, entao adiciona 7 as centenas 6 dezena e 8 unidade
	add valor_centena, r20
	ldi r20,0x06
	add valor_dezena, r20
	ldi r20,0x08
	add valor_unidade, r20
	jmp verifica_carry
	
verifica_carry:
	mov r20, valor_unidade 		; verifica se o valor da unidade e maior que 9
	cpi r20, 10
	brsh add_dezena
	mov r20, valor_dezena		; verifica se o valor da dezena e maior que 9
	cpi r20, 10
	brsh add_centena
	ret

add_dezena:
	inc valor_dezena
	ret

add_centena:
	inc valor_centena
	ret
; =======================================================
; 						f_show_cont
; =======================================================	
f_show_cont:
	clr r20

    cpi cont_pov, 3
    breq mostrar_centena
    cpi cont_pov, 2
    breq mostrar_dezena
    cpi cont_pov, 1
    breq mostrar_unidade
	ret

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
; =======================================================
; 						f_mde
; =======================================================
f_mde:
    cpi estado, 0x00
    breq estado_0
    cpi estado, 0x01
    breq estado_1
    cpi estado, 0x02
    breq estado_2
    cpi estado, 0x03
    breq estado_3
    cpi estado, 0x04
    breq estado_4
    ret

estado_0:
    ; inicial
    call f_RGB_off
    ret

estado_1:
    ; decrescente
    call f_RGB_off
    ret

estado_2:
    ; config_min
    call f_RGB_R
	call f_leitura_pot

	mov XL, r20
	mov XH, r21

	mov ZL, r20
	mov ZH, r21
    ret

estado_3:
    ; config_max
    call f_RGB_G
	call f_leitura_pot

	mov YL, r20
	mov YH, r21

	mov ZL, r20
	mov ZH, r21
    ret

estado_4:
    ; config_passo
    call f_RGB_B
	call f_leitura_pot

	mov valor_passo, r20

	mov ZL, r20
	mov ZH, r21
    ret
; =======================================================
; 						F_RGB
; =======================================================
f_RGB:
    ldi r20, 0x0e   ; PB1-PB3 como saida
    out DDRB, r20 
    ret

f_RGB_off:
    cbi PORTB, 1    ; PB1 LOW
    cbi PORTB, 2    ; PB2 LOW
    cbi PORTB, 3    ; PB3 LOW
    ret

f_RGB_R:
	cbi PORTB, 1    ; PB1 LOW
    cbi PORTB, 2    ; PB2 LOW
    sbi PORTB, 3    ; PB3 HIGH
    ret

f_RGB_G:
    cbi PORTB, 1    ; PB1 LOW
    sbi PORTB, 2    ; PB2 HIGH
    cbi PORTB, 3    ; PB3 LOW
    ret

f_RGB_B:
	sbi PORTB, 1    ; PB1 HIGH
    cbi PORTB, 2    ; PB2 LOW
    cbi PORTB, 3    ; PB3 LOW
    ret
; =======================================================
; 						PCINT1_ISR
; =======================================================
PCINT1_ISR:
	clr r24

	in r24, PINC						; le o PINC
	andi r24,0x0e						; mascara para pegar os 3 bits de interesse
	cpi r24,0x06						; S3 pressionado
	breq case_cont_decrescente			
	cpi r24,0x0a
	breq case_ajusta_valores			; S2 pressionado
	cpi r24,0x0c
	breq case_cont_crescente			; S1 pressionado
	reti

case_cont_decrescente:
	ldi estado, 0x01
    reti
case_ajusta_valores:
	cpi estado, 0x04					; volta ao estado inicial apos o btn S3 ser pressionado 3 vezes
	breq case_cont_crescente

	ldi r16, 0x01
	add estado, r16
    reti
case_cont_crescente:
	ldi estado, 0x00
    reti
; =======================================================
; 						f_leitura_pot
; =======================================================
f_leitura_pot:
    ldi     r20, 0xC7    
	sts     ADCSRA,r20   				; inicia a conversao 
loop_adc:
	lds     r20, ADCSRA  				; carrega no registrador 20 o valor de ADCSRA
	sbrs    r20, ADIF   	 			; verifique se o processo de conversao finalizou
	rjmp    loop_adc 
	lds     r6, ADCL    				; carrega o valor L do conversor ad no r6
	lds     r7, ADCH    				; carrega o valor H do conversor ad no r7

converte_valor_adc:
    clr     r20     					; resultado parte low
    clr     r21     					; resultado parte high

	cpi estado,0x04
	rjmp escala_passo 					; valores de 0 a 15

    ldi  r22, low(1024)
    ldi  r23, high(1024)    			; limiar
	rjmp loop_divisao_simples

escala_passo:
	swap r7								; 0000 00XX -> 00XX 0000 

	lsr r6
	lsr r6
	lsr r6
	lsr r6								; depois de 4 lsr: XXXX XXXX -> 0000 XXXX

	or r6,r7							; 0000 XXXX + 00XX 0000 = 00XX XXXX
	lsr r6
	lsr r6								; 00XX XXXX -> 0000 XXXX

	clr r7
	mov r20, r6
	mov r21, r7
	ret

loop_divisao_simples:
    cp      r6, r22
    cpc     r7, r23
    brlo    final_divisao

    sub     r6, r22
    sbc     r7, r23

    inc     r20				
    cpi     r20, 0
    brne    loop_divisao_simples
    inc     r21
    rjmp    loop_divisao_simples

final_divisao:
    ret
    

    