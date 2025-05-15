.include "m328pdef.inc"

.def cont_pov = r18
.def cont_cont_pov = r21
.def valor_centena = r0
.def valor_dezena = r1
.def valor_unidade = r2
.def estado = r17

.cseg
.org 0x0000
    rjmp main
.org 0x008 							; interrupcao PCINT1 
	jmp PCINT1_ISR

main:
	ldi estado, 0x00    			; estado inicial
	ldi r16, 0x10 
	out DDRC, r16	  				; define os pinos PC1 a PC3 e PC5 como entrada, btn e SCL (echo)

	ldi r16,0x02 					; define interrupcao botao
	sts PCICR, r16
	ldi r16,0x0E
	sts PCMSK1,r16
	sei

	ldi r20, 0xfc
    out DDRD, r20  					; define os pinos PD7-PD2 como saida
	sbi DDRB,0      				; define PB0 como saida

    ldi cont_pov, 3   				; contador de 3 elementos para o efeito pov
	clr cont_cont_pov  				; contador da quantidade de pov 

	ldi ZL, low(400)				; pequeno teste para ver se funciona o salvamento na eeprom
	ldi ZH, high(400)
	call f_escrever_eepromL
	call f_escrever_eepromH
	clr ZL
	clr ZH
loop: 
	call f_valor     				; calcula a quantidade de centena, dezena e unidade a ser exibida
    call f_show_cont 				; exibe contador
    call f_pov       				; faz o efeito pov
	call f_mde	 					; maquina de estados 
	call f_delay     				; delay para o contador
	inc cont_cont_pov

    rjmp loop

.include "f_pov.asm"
.include "f_valor.asm"
.include "f_show_cont.asm"
.include "f_delay.asm"
.include "f_mde.asm"
.include "HCSR04.asm"
.include "PCINT1_ISR.asm"
.include "EEPROM.asm"


    