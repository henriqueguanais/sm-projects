#include <avr/io.h>
#include "gerador.h"

static const uint8_t COS_TABLE[32] = {
    255, 253, 246, 234, 218, 199, 177, 153,
    128, 103, 79, 57, 38, 22, 10, 3,
    0, 3, 10, 22, 38, 57, 79, 103,
    128, 153, 177, 199, 218, 234, 246, 253};

void envia_dados(uint16_t y)
{
    PORTC = (y << 4) & 0x30; // Envia os bits menos significativos para PC5 e PC6 0000 00xx
    PORTB = (y >> 2);        // Envia os bits mais significativos para PB0 a PB5  xxxx xx00
}

void quadrada(uint8_t amp, uint8_t offset) {
    static uint16_t y = 0;

    if (y == offset) {
        y += amp;
    }
    else if (y == (offset + amp)) {
        y = offset;
    }

    if (y >= 255) {
        y = 255; // Limita o valor máximo
    }
    else if (y <= 0) {
        y = 0; // Limita o valor mínimo
    }

    envia_dados(y);
}

void triangular(uint8_t amp, uint8_t offset) {
    static uint16_t y = 0;
    static uint8_t direction = 1; // 1 para subir, 0 para descer
    
    if ((y < amp + offset) && direction == 1) {
        y += 1; // Incrementa até atingir o pico
    }
    else {
        y -= 1; // Decrementa até voltar ao offset
        direction = 0; // Muda a direção para descer
    }

    if (y == offset) {
        direction = 1; // Muda a direção para subir
    }

    envia_dados(y);
}

void senoidal(uint8_t amp, uint8_t offset, uint8_t index) {
    static uint16_t y = 0;

    // Calcula o valor da senoide usando a tabela de cosseno
    y = COS_TABLE[index];

    envia_dados(y);
    
}

void rampa(uint8_t amp, uint8_t offset) {
    static uint16_t y = 0;

    if (y < amp + offset) {
        y += 1; // Incrementa até atingir o pico
    }
    else {
        y = offset; // Reseta para o offset

    }

    envia_dados(y);

}