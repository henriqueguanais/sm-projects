#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <stdio.h>

#include "LCD.h"
#include "joystick.h"
#include "MAX7219.h"

#define ESTADO0 0
#define ESTADO1 1
#define ESTADO2 2
#define ESTADO3 3
#define ESTADO4 4

volatile uint8_t estado_mde = ESTADO0;
volatile int8_t passo = 1;       // passo de incremento, decremento
volatile int8_t muda_estado = 0; // 0 = não muda, 1 = muda
volatile int8_t cresce = 0;      // 0 = não cresce, 1 = cresce
volatile uint16_t cont = 0;
volatile uint8_t tamanho_cobra = 1; // Tamanho inicial da cobra

void setup();
void mde(uint8_t *pos_x, uint8_t *pos_y, uint8_t *direction_vector);
void aumenta_cobra(uint8_t *pos_x, uint8_t *pos_y);

int main()
{
    setup();
    setup_joystick();
    max7219_init();

    uint8_t pos_x[8] = {3, 0, 0, 0, 0, 0, 0, 0};
    uint8_t pos_y[8] = {4, 0, 0, 0, 0, 0, 0, 0};

    char pos_x_str[2];
    char pos_y_str[2];

    inic_LCD_4bits();  // inicializa o LCD
    escreve_LCD("X:"); // string armazenada na RAM
    cmd_LCD(0xC0, 0);  // desloca cursor para a segunda linha
    escreve_LCD("Y:"); // string armazenada na RAM
    cmd_LCD(0x83, 0);

    // Atualiza as posições do vetor
    cmd_LCD(0x83, 0); // linha x, comeco do vetor
    for (int i = 0; i < 7; i++)
    {
        ident_num(pos_x[i], pos_x_str);
        escreve_LCD(&pos_x_str[0]);
        escreve_LCD(" ");
    }
    cmd_LCD(0xC3, 0); // linha y
    for (int i = 0; i < 7; i++)
    {
        ident_num(pos_y[i], pos_y_str);
        escreve_LCD(&pos_y_str[0]);
        escreve_LCD(" ");
    }

    uint16_t vy_joystick = 0;
    uint16_t vx_joystick = 0;
    uint8_t direction_vector[4] = {0};
    max7219_clear_matrix(); // Limpa a matriz LED

    while (1)
    {
        cmd_LCD(0x02, 0); // retorna cursor para o início da linha

        set_matriz_leds(pos_x, pos_y);
        // Atualiza as posições do vetor
        cmd_LCD(0x83, 0); // linha x, comeco do vetor
        for (int i = 0; i < 7; i++)
        {
            ident_num(pos_x[i], pos_x_str);
            escreve_LCD(&pos_x_str[0]);
            escreve_LCD(" ");
        }
        cmd_LCD(0xC3, 0); // linha y
        for (int i = 0; i < 7; i++)
        {
            ident_num(pos_y[i], pos_y_str);
            escreve_LCD(&pos_y_str[0]);
            escreve_LCD(" ");
        }

        ler_joystick(&vy_joystick, &vx_joystick);

        joystick_direction(vy_joystick, vx_joystick, direction_vector);
        mde(pos_x, pos_y, direction_vector);
    }
}

void setup()
{
    DDRD |= 0xFC; // setando PD2, PD3 PD4 a PD7 como saida, LCD
    DDRB |= 0x2D; // setando PB2, PB3, PB5, MAX7219 como saida

    TCCR0A = 0x00; // timer de 16 ms
    TCCR0B = 0xC5; // prescaler de 1024
    TCNT0 = 0;
    TIMSK0 = (1 << TOIE0);
    sei();
}

void mde(uint8_t *pos_x, uint8_t *pos_y, uint8_t *direction_vector)
{
    if (muda_estado)
    {
        muda_estado = 0;
        if (estado_mde == ESTADO0)
        {
            estado_mde = ESTADO4; // RIGHT
        }
        if (direction_vector[0] == 1)
        {
            estado_mde = ESTADO1; // UP
        }
        if (direction_vector[1] == 1)
        {
            estado_mde = ESTADO2; // DOWN
        }
        if (direction_vector[2] == 1)
        {
            estado_mde = ESTADO3; // LEFT
        }
        if (direction_vector[3] == 1)
        {
            estado_mde = ESTADO4; // RIGHT
        }

        if (pos_x[0] >= 8 || pos_x[0] == 0 || pos_y[0] >= 8 || pos_y[0] == 0)
        {
            estado_mde = ESTADO0; // reseta se passar dos limites
        }

        switch (estado_mde)
        {
        case ESTADO0:
            // posiçao inicial
            // Animação de reset
            animacao_game_over();
            max7219_clear_matrix(); // Limpa a matriz LED
            direction_vector[0] = 0;
            direction_vector[1] = 0;
            direction_vector[2] = 0;
            direction_vector[3] = 1;
            tamanho_cobra = 1; // Reseta o tamanho da cobra
            for (int i = 0; i < 8; i++)
            {
                pos_x[i] = 0;
                pos_y[i] = 0;
            }
            pos_x[0] = 3; // inicio da cobra
            pos_y[0] = 4; // inicio da cobra

            break;

        case ESTADO1:
            // UP
            aumenta_cobra(pos_x, pos_y); // Aumenta a cobra antes de mudar a posição
            pos_y[0] += 1;
            break;

        case ESTADO2:
            // DOWN
            aumenta_cobra(pos_x, pos_y); // Aumenta a cobra antes de mudar a posição
            pos_y[0] -= 1;
            break;

        case ESTADO3:
            // LEFT
            aumenta_cobra(pos_x, pos_y); // Aumenta a cobra antes de mudar a posição
            pos_x[0] -= 1;
            break;

        case ESTADO4:
            // RIGHT
            aumenta_cobra(pos_x, pos_y); // Aumenta a cobra antes de mudar a posição
            pos_x[0] += 1;
            break;
        default:
            break;
        }
    }

    _delay_ms(100);
}

void aumenta_cobra(uint8_t *pos_x, uint8_t *pos_y)
{
    // Aumenta a cobra
    for (int i = tamanho_cobra - 1; i > 0; i--)
    {
        pos_x[i] = pos_x[i - 1];
        pos_y[i] = pos_y[i - 1];
    }
}

ISR(TIMER0_OVF_vect)
{
    static uint8_t timer_cobra = 0;

    cont++;
    if (cont >= 32)
    {
        // 0.5 s
        cont = 0;

        if (timer_cobra >= 5)
        {
            cresce = 1;      // cresce a cobra a cada 2 segundos
            timer_cobra = 0; // reseta o timer

            if (tamanho_cobra < 8)
            {
                tamanho_cobra++; // aumenta o tamanho da cobra
            }
            else
            {
                tamanho_cobra = 8; // limita o tamanho da cobra a 8
            }
        }
        else
        {
            timer_cobra++;
        }

        muda_estado = 1;
    }
}
