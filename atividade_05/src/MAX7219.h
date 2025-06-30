#ifndef MAX7219_H
#define MAX7219_H

#include <avr/io.h>

// Definições do hardware
#define MAX_DIN PB3 // Pino SS (Chip Select)
#define MAX7219_PORT PORTB
#define MAX7219_DDR DDRB
#define MAX_CS PB2    // Pino de controle do MAX7219 (Chip Select)
#define MAX_CLOCK PB5 // Pino de clock do MAX7219

// Endereços de registro do MAX7219 (datasheet)
#define MAX7219_DECODE_MODE 0x09
#define MAX7219_INTENSITY 0x0A
#define MAX7219_SCAN_LIMIT 0x0B
#define MAX7219_SHUTDOWN 0x0C
#define MAX7219_DISPLAY_TEST 0x0F

// Buffer da matriz LED (8x8)
extern uint8_t led_matrix[8];

// Protótipos de funções
void max7219_init(void);
void max7219_send_command(uint8_t address, uint8_t data);
void max7219_update_matrix(void);
void max7219_clear_matrix(void);
void set_matriz_leds(uint8_t *pos_x, uint8_t *pos_y);
void set_all_leds();
void animacao_game_over();

#endif