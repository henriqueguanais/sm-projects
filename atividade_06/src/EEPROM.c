#include <avr/io.h>
#include "EEPROM.h"

void escrever_eeprom(uint8_t endereco, unsigned char dado) {
    while(EECR & (1<<EEPE)); // Espera até que a EEPROM esteja pronta para escrita
    EEAR = endereco; // Define o endereço da EEPROM
    EEDR = dado; // Define o dado a ser escrito
    EECR |= (1<<EEMPE); // Habilita a escrita na EEPROM
    EECR |= (1<<EEPE); // Inicia a escrita
}

unsigned char ler_eeprom(uint8_t endereco) {
    while(EECR & (1<<EEPE)); // Espera até que a EEPROM esteja pronta para leitura
    EEAR = endereco; // Define o endereço da EEPROM
    EECR |= (1<<EERE); // Inicia a leitura
    return EEDR; // Retorna o dado lido
}

void salva_vetores(uint8_t *vetor, uint8_t tamanho) {
    for (uint8_t i = 0; i < tamanho; i++) {
        escrever_eeprom(i, vetor[i]);
    }
    escrever_eeprom(0xFF, 0xF0); // Flag para indicar que os coeficientes foram salvos
}

void ler_vetores(uint8_t *vetor, uint8_t tamanho) {
    if (ler_eeprom(0xFF) == 0xF0) { // so le se a flag indicar que os coeficientes foram salvos
        for (uint8_t i = 0; i < tamanho; i++) {
        vetor[i] = ler_eeprom(i);
        }
    }
    else {
        for (uint8_t i = 0; i < tamanho; i++) {
            vetor[i] = 0; // Se não houver dados salvos, inicializa com zero
        }
    }
    
}