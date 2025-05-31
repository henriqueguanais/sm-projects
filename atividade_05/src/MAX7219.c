#include "MAX7219.h"
#include <avr/io.h>

uint8_t led_matrix[8] = {0}; // Buffer da matriz

// Inicializa o MAX7219 e a comunicação SPI
void max7219_init(void) {
    // Configuração do pino SS
    MAX7219_DDR |= (1 << MAX7219_SS);
    MAX7219_PORT |= (1 << MAX7219_SS);

    // Configuração do SPI (Modo 0, MSB first)
    SPCR = (1 << SPE) | (1 << MSTR); // Master, Fosc/4
    SPSR |= (1 << SPI2X); // Dobra a velocidade (Fosc/2)

    // Configuração inicial do MAX7219
    max7219_send_command(MAX7219_SHUTDOWN, 0x01);      // Sai do modo shutdown
    max7219_send_command(MAX7219_DECODE_MODE, 0x00);   // Sem decodificação
    max7219_send_command(MAX7219_SCAN_LIMIT, 0x07);    // Digitaliza todos os 8 dígitos
    max7219_send_command(MAX7219_INTENSITY, 0x0F);     // Intensidade máxima
    max7219_send_command(MAX7219_DISPLAY_TEST, 0x00);  // Desliga teste
}

// Envia um comando para o MAX7219 via SPI
void max7219_send_command(uint8_t address, uint8_t data) {
    MAX7219_PORT &= ~(1 << MAX7219_SS); // Ativa o chip

    SPDR = address; // Envia endereço
    while(!(SPSR & (1 << SPIF))); // Espera transmissão

    SPDR = data; // Envia dado
    while(!(SPSR & (1 << SPIF))); 

    MAX7219_PORT |= (1 << MAX7219_SS); // Desativa o chip
}

// Atualiza a matriz LED com o buffer
void max7219_update_matrix(void) {
    for(uint8_t i = 0; i < 8; i++) {
        max7219_send_command(i + 1, led_matrix[i]); // DIG0 a DIG7
    }
}

// Limpa a matriz LED
void max7219_clear_matrix(void) {
    for(uint8_t i = 0; i < 8; i++) {
        led_matrix[i] = 0x00;
    }
    max7219_update_matrix();
}