#include "MAX7219.h"
#include <avr/io.h>
#include <util/delay.h>

uint8_t led_matrix[8] = {0}; // Buffer da matriz

// Inicializa o MAX7219 e a comunicação SPI
void max7219_init(void)
{
    // Configuração do pino SS
    MAX7219_DDR |= (1 << MAX_DIN) | (1 << MAX_CS) | (1 << MAX_CLOCK); // Configura pinos como saída
    MAX7219_PORT |= (1 << MAX_CS);

    // Configuração do SPI (Modo 0, MSB first)
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

    // Configuração inicial do MAX7219
    max7219_send_command(MAX7219_SHUTDOWN, 0x01);     // Sai do modo shutdown
    max7219_send_command(MAX7219_DECODE_MODE, 0x00);  // Sem decodificação
    max7219_send_command(MAX7219_SCAN_LIMIT, 0x07);   // Digitaliza todos os 8 dígitos
    max7219_send_command(MAX7219_INTENSITY, 0x0F);    // Intensidade máxima
    max7219_send_command(MAX7219_DISPLAY_TEST, 0x00); // Desliga teste
}

// Envia um comando para o MAX7219 via SPI
void max7219_send_command(uint8_t address, uint8_t data)
{
    MAX7219_PORT &= ~(1 << MAX_CS); // Ativa o chip

    SPDR = address; // Envia endereço
    while (!(SPSR & (1 << SPIF))); // Espera transmissão

    SPDR = data; // Envia dado
    while (!(SPSR & (1 << SPIF)));

    MAX7219_PORT |= (1 << MAX_CS); // Desativa o chip
}

// Atualiza a matriz LED com o buffer
void max7219_update_matrix(void)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        max7219_send_command(i, led_matrix[i]); // DIG0 a DIG7
    }
}

// Limpa a matriz LED
void max7219_clear_matrix(void)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        led_matrix[i] = 0x00;
    }
    max7219_update_matrix();
}

// Liga um conjunto de LEDs na matriz, dado um vetor de posições x e y
void set_matriz_leds(uint8_t *pos_x, uint8_t *pos_y)
{
    max7219_clear_matrix(); // Limpa a matriz antes de atualizar
    for (uint8_t i = 0; i < 8; i++)
    {
        if (pos_x[i] < 8 && pos_y[i] < 8)
        {                                            // Verifica se as posições estão dentro do limite
            led_matrix[pos_y[i]] |= (1 << pos_x[i]); // Liga o LED na posição especificada
        }
        max7219_update_matrix(); // Atualiza a matriz com os novos valores
    }
}

// Liga todos os LEDs da matriz
void set_all_leds()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        led_matrix[i] = 0xFF; // Liga todos os LEDs
    }
    max7219_update_matrix(); // Atualiza a matriz com os novos valores
}

void animacao_game_over()
{
    max7219_clear_matrix();
    for (int i = 0; i <= 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            led_matrix[i] |= (1 << j); // Liga todos os LEDs
            max7219_update_matrix();
            _delay_ms(10); // Delay para animação
        }
    }
    for (int i = 0; i <= 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            led_matrix[i] &= ~(1 << j); // Desliga todos os LEDs
            max7219_update_matrix();
            _delay_ms(10); // Delay para animação
        }
    }
}