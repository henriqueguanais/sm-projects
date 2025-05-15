f_mde:
    cpi estado, 0x00
    breq estado_0
    cpi estado, 0x01
    breq estado_1
    cpi estado, 0x02
    breq estado_2
    cpi estado, 0x03
    breq estado_3
    ret
estado_0:
    ; exibi valor
    ret
estado_1:
    ; mede distancia
	call HCSR04 
	ldi estado, 0x00
    ret
estado_2:
    ; salva memoria
	ldi estado, 0x00
	call f_escrever_eepromL
	call f_escrever_eepromH

    ret
estado_3:
    ; exibe memoria 
	ldi estado, 0x00
	call f_ler_eepromL
	call f_ler_eepromH
    ret

