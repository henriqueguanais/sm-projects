#include <avr/io.h> //definições do componente especificado
#include <util/delay.h> //biblioteca para o uso das rotinas de _delay_ms e _delay_us()

#include "joystick.h"

// Configura o joystick
void setup_joystick() {
    DDRC &= ~0x30;// setando PC4 e PC5 como entrada, joystick

    ADMUX  = (1<<REFS0);     // configura ADC           
	ADCSRA = 0x07;                      
	ADCSRB = 0x00;                      
	ADCSRA|= (1<<ADEN); 
}

// Le o eixo y e x do joystick
void ler_joystick(uint16_t *valor_y, uint16_t *valor_x) {
    ADMUX |= 0x04;      // ADC4 
    ADCSRA|=(1<<ADSC); // inicia a conversao
    while(!(ADCSRA & (1<<ADIF)));   // espera a conversao terminar

    *valor_y = ADC; // armazena o valor lido
    _delay_us(10); 

    ADMUX |= 0x05;      // ADC5
    ADCSRA|=(1<<ADSC); // inicia a conversao
    while(!(ADCSRA & (1<<ADIF)));   // espera a conversao terminar

    *valor_x = ADC; // armazena o valor lido
    _delay_us(10);
    ADMUX &= ~0x30; // limpa os bits de seleção do canal
}

// Verifica a direcao do joystick
void joystick_direction(uint16_t valor_y, uint16_t valor_x, uint8_t *direction_vector) {
    if (valor_y >= LIM_INF_DZ && valor_y <= LIM_SUP_DZ) {
        direction_vector[0] = 0;
        direction_vector[1] = 0;
    }
    else if (valor_y >= LIM_SUP_DZ) {
        direction_vector[0] = 1;    // up
        direction_vector[1] = 0;
    }
    else {
        direction_vector[0] = 0;
        direction_vector[1] = 1;    // down
    }

    if (valor_x >= LIM_INF_DZ && valor_x <= LIM_SUP_DZ) {
        direction_vector[3] = 0;
        direction_vector[4] = 0;
    }
    else if (valor_x >= LIM_SUP_DZ) {
        direction_vector[3] = 0;    
        direction_vector[4] = 1;    // right
    }
    else {
        direction_vector[3] = 1;     // left
        direction_vector[4] = 0;    
    }
}
