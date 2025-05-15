f_ler_lista:
    cpi cont_cont_pov, 250          ; so le o proximo depois de 250 contagens
    breq ler_prox_numero
    ret         

ler_prox_numero:
	clr cont_cont_pov
    lpm r16, Z+                     ; carrega o valor da lista
    dec cont_lista                  ; decrementa o contador de lista
    cpi cont_lista, 0               ; verifica se chegou ao fim da lista
    breq reset_lista                ; volta para o inicio da lista
    ret
    
reset_lista:
    ; reseta a lista
    ldi ZL, low(2*lista)
    ldi ZH, high(2*lista)
    ldi cont_lista, 10              ; reseta o contador de 10 elementos para a lista
    ret
