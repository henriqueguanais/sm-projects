f_escrever_eepromL:
	ldi r16, 0x00
    sbic EECR, EEPE                 ; Espera se EEPROM estiver ocupada
    rjmp f_escrever_eepromL

    out EEARL, r16                  ; Define endereco 
	out EEARH, r16                  ; Define endereco

    out EEDR, ZL                    ; Define dado
    sbi EECR, EEMPE                 ; Habilita gravacao
    sbi EECR, EEPE                  ; Inicia gravacao
    ret

f_escrever_eepromH:
	sbic EECR, EEPE                 ; Espera se EEPROM estiver ocupada
    rjmp f_escrever_eepromH

	out EEARH, r16                  ; Define endereco
	ldi r16, 0x01
	out EEARL, r16                  ; Define endereco 

    out EEDR, ZH                    ; Define dado
    sbi EECR, EEMPE                 ; Habilita gravacao
    sbi EECR, EEPE                  ; Inicia gravacao
    ret

f_ler_eepromL:
	ldi r16, 0x00
    sbic EECR, EEPE                 ; Espera se EEPROM estiver ocupada
    rjmp f_ler_eepromL

    out EEARL, r16
	out EEARH, r16

    sbi EECR, EERE                  ; Dispara leitura
    in ZL, EEDR                     ; Le valor
    ret

f_ler_eepromH:
	sbic EECR, EEPE                 ; Espera se EEPROM estiver ocupada
    rjmp f_ler_eepromH

	out EEARH, r16
	ldi r16, 0x01
	out EEARL, r16  

    sbi EECR, EERE                  ; Dispara leitura
    in ZH, EEDR                     ; Le valor
    ret