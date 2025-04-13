f_valor:
    ; calcula o valor da centena, dezena e unidade
    clr r20
    mov r20, r16 ; copia r16 para r20
    clr valor_centena   ; contador de centena
    clr valor_dezena   ; contador de dezena
    clr valor_unidade   ; contador de unidade
cent:
	cpi r20,100 ; compara se o numero e igual a 100
	brsh decrementa100  ; se for maior que 100 vai para decrementa100
	rjmp dez
decrementa100:
	subi r20,100 ; subtrai 100 do valor
	inc valor_centena      ; incrementa o contador de centena
    rjmp cent
dez:
	cpi r20,10  ; compara se o numero e igual a 10
	brsh decrementa10  ; se for maior que 10 vai para decrementa10
	rjmp uni
decrementa10:
	subi r20,10  ; subtrai 10 do valor
	inc valor_dezena    ; incrementa o contador de dezena
	cpi r20,10   ; compara com 10 
	rjmp dez  
uni:
	cpi r20,1   ; compara com 1
	brsh decrementa1   ; se for maior que 1 vai para decrementa1
	ret 
decrementa1:
	subi r20,1  ; subtrai 1 do valor
	inc valor_unidade   ; incrementa o contador de unidade
	rjmp uni   
    