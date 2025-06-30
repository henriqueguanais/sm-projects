#include "FIR.h"
#include <avr/io.h>
#include "ADC.h"

uint8_t filtro_fir(const float *coefs)
{
    static float x_buffer[16] = {0}; // buffer para armazenar os valores de entrada
    float x_result = 0;
    float y_result = 0;
    uint8_t y = 0;
    uint16_t x = 0;

    ler_adc(&x);

    x_result = ((float)x - 512.0) / 512.0;

    for (uint8_t i = 0; i < 15; i++)
    {
        x_buffer[i + 1] = x_buffer[i]; // desloca os valores do buffer
    }
    x_buffer[0] = x_result;
    y_result = 0;
    for (uint8_t i = 0; i < 16; i++)
    {
        y_result += x_buffer[i] * coefs[i];
    }
    y_result = (y_result * 127.0) + 127.0;

    y = (uint8_t)y_result;

    return y;
}

void envia_dados(uint8_t y)
{
    PORTC = (y << 4) & 0x30; // Envia os bits menos significativos para PC5 e PC6 0000 00xx
    PORTB = (y >> 2);        // Envia os bits mais significativos para PB0 a PB5  xxxx xx00
}
