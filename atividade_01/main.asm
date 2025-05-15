.include "m328pdef.inc"

.def cont_lista = r17
.def cont_pov = r18
.def cont_cont_pov = r24
.def valor_centena = r21
.def valor_dezena = r22
.def valor_unidade = r23

.cseg
.org 0x0000
    rjmp main

lista : .db 10,54,22,244,0,255,15,54,77,111       ; lista de 10 elementos

main:
    ; inicializacao dos pinos
	ldi r26, 0xfc
    out DDRD, r26           ; define os pinos PD7-PD2 como saida
	sbi DDRB,0              ; define PB0 como saida

    ; carregar valores da lista
    ldi ZL, low(2*lista)
    ldi ZH, high(2*lista)
    
    ldi cont_lista, 10      ; contador de 10 elementos para a lista
    ldi cont_pov, 3         ; contador de 3 elementos para o efeito pov
	clr cont_cont_pov       ; contador da quantidade de pov 
loop: 
    call f_ler_lista        ; carrega o valor da lista
	call f_valor            ; calcula a quantidade de centena, dezena e unidade a ser exibida
    call f_show_cont        ; exibe contador
    call f_pov              ; faz o efeito pov
	call f_delay            ; delay para o pov

    rjmp loop

.include "f_ler_lista.asm"
.include "f_pov.asm"
.include "f_valor.asm"
.include "f_show_cont.asm"
.include "f_delay.asm"
    