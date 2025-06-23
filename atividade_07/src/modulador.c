#include <avr/io.h>
#include <util/delay.h>

#include "modulador.h"

float passo = 0;
const uint8_t COS_TABLE[32] = {
    255, 253, 246, 234, 218, 199, 177, 153,
    128, 103,  79,  57,  38,  22,  10,   3,
      0,   3,  10,  22,  38,  57,  79, 103,
    128, 153, 177, 199, 218, 234, 246, 253
};

uint8_t modula_am(uint16_t freq_portadora, uint16_t msg)
{
    uint8_t sinal_modulado = 0;
    static uint8_t cont = 0;

    if (cont >= 32) {
        cont = 0; // Reseta o contador quando atinge o tamanho da tabela
    }
    sinal_modulado = COS_TABLE[cont];
    cont++;

    return sinal_modulado;
}

uint8_t modula_fm(uint16_t freq_portadora, uint16_t msg)
{
    uint8_t sinal_modulado = 0;
    return 0;
}

uint8_t modula_ask(uint16_t freq_portadora, uint16_t msg)
{
    uint8_t sinal_modulado = 0;
    return 0;
}
uint8_t modula_fsk(uint16_t freq_portadora, uint16_t msg)
{
    uint8_t sinal_modulado = 0;
    return 0;
}

void envia_dados(uint8_t y)
{
    PORTC = (y << 4) & 0x30; // Envia os bits menos significativos para PC5 e PC6 0000 00xx
    PORTB = (y >> 2);        // Envia os bits mais significativos para PB0 a PB5  xxxx xx00
}
