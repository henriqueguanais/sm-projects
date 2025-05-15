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
	inc valor_unidade   		; incrementa o contador de unidade
	rjmp uni   
    
comp_ZH:
	cpi ZH,0x01					; analisa o valor de ZH
	breq case_ZH1
	ret

case_ZH1:
	ldi r20,0x02				; se ZH for 00000001, entao adiciona 2 as centenas 5 dezena e 6 unidade
	add valor_centena, r20
	ldi r20,0x05
	add valor_dezena, r20
	ldi r20,0x06			
	add valor_unidade, r20

	mov r20, valor_unidade 		; verifica se o valor da unidade e maior que 9
	cpi r20, 10
	brsh add_dezena
	
	mov r20, valor_dezena		; verifica se o valor da dezena e maior que 9
	cpi r20, 10
	brsh add_centena
	ret

add_dezena:
	inc valor_dezena
	mov r20, valor_dezena

	cpi r20, 10
	brsh add_centena
	ret

add_centena:
	inc valor_centena
	ret
	