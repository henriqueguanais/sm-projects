#ifndef EEPROM_H
#define EEPROM_H

void escrever_eeprom(uint8_t endereco, unsigned char dado);
unsigned char ler_eeprom(uint8_t endereco);
void salva_vetores(uint8_t *vetor, uint8_t tamanho);
void ler_vetores(uint8_t *vetor, uint8_t tamanho) ;

#endif