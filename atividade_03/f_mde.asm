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
    ; exibe valor
    ret
estado_1:
    ; mede distancia
	call HCSR04 
	ldi estado, 0x00                ; retorna para o estado 0
    ret
estado_2:
    ; salva memoria
	call f_escrever_eepromL
	call f_escrever_eepromH
    ldi estado, 0x00                ; retorna para o estado 0
    ret
estado_3:
    ; exibe memoria 
	call f_ler_eepromL
	call f_ler_eepromH
    ldi estado, 0x00                ; retorna para o estado 0
    ret

