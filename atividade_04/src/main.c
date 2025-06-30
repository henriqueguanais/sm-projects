#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "LCD.h"

#define ESTADO0 0
#define ESTADO1 1
#define ESTADO2 2
#define ESTADO3 3

volatile uint8_t estado_mde = ESTADO0;
volatile int8_t passo = 1;   // passo de incremento, decremento
volatile int8_t inc_dec = 0; // -1 = decremento, 1 = incremento, 0 = nada
volatile uint16_t cont = 0;

void setup();
void mde(uint8_t *red_color, uint8_t *green_color, uint8_t *blue_color);

int main()
{
    setup();

    uint8_t red_color = 0;
    uint8_t green_color = 0;
    uint8_t blue_color = 0;

    char red_color_str[4] = {};
    char green_color_str[4] = {};
    char blue_color_str[4] = {};

    inic_LCD_4bits();               // inicializa o LCD
    escreve_LCD(" RED GREEN BLUE"); // string armazenada na RAM
    cmd_LCD(0xC0, 0);               // desloca cursor para a segunda linha

    while (1)
    {
        cmd_LCD(0x02, 0); // retorna cursor para o início da linha

        ident_num(red_color, red_color_str);
        cmd_LCD(0xC1, 0);
        escreve_LCD(red_color_str);
        if (red_color < 100)
        {
            cmd_LCD(0xC1, 0);
            escreve_LCD(" "); // limpa o caractere da centena

            if (red_color < 10)
            {
                cmd_LCD(0xC2, 0); // posicao da dezena
                escreve_LCD(" "); // limpa o caractere da dezena
            }
        }

        ident_num(green_color, green_color_str);
        cmd_LCD(0xC6, 0);
        escreve_LCD(green_color_str);
        if (green_color < 100)
        {
            cmd_LCD(0xC6, 0);
            escreve_LCD(" "); // limpa o caractere da centena
            if (green_color < 10)
            {
                cmd_LCD(0xC7, 0); // posicao da dezena
                escreve_LCD(" "); // limpa o caractere da dezena
            }
        }

        ident_num(blue_color, blue_color_str);
        cmd_LCD(0xCB, 0);
        escreve_LCD(blue_color_str);
        if (blue_color < 100)
        {
            cmd_LCD(0xCB, 0);
            escreve_LCD(" "); // limpa o caractere da centena
            if (blue_color < 10)
            {
                cmd_LCD(0xCC, 0); // posicao da dezena
                escreve_LCD(" "); // limpa o caractere da dezena
            }
        }

        mde(&red_color, &green_color, &blue_color);
    }
}

void setup()
{
    DDRB |= 0x0E;  // setando PB1 a PB3 como saida, LEDS
    DDRC &= ~0x0E; // setando PC1 a PC3 como entrada, BTNS
    DDRD |= 0xFC;  // setando PD2, PD3 PD4 a PD7 como saida, LCD

    PCICR = 0x02; // interrupcao btn
    PCMSK1 = 0x0E;

    TCCR0A = 0x00; // timer de 16 ms
    TCCR0B = 0xC5; // prescaler de 1024
    TCNT0 = 0;
    TIMSK0 = (1 << TOIE0);
    sei();

    TCCR2A = 0x83; // fast pwm para OCR2A
                   // TCCR2A = 0xD3;   // fast pwm para OCR2A
    TCCR2B = 0x01;
    TCNT2 = 0;
    OCR2A = 0; // inicia contagem em 0

    TCCR1A = 0xA1; // fast pwm para OCR1A e OCR1B
    TCCR1B = 0x09; // fast pwm, no prescaling
    TCNT1 = 0;     // inicia contagem em 0
    OCR1A = 0;     // inicia contagem em 0
    OCR1B = 0;     // inicia contagem em 0
}

void mde(uint8_t *red_color, uint8_t *green_color, uint8_t *blue_color)
{
    switch (estado_mde)
    {

    case ESTADO0:
        cmd_LCD(0xCE, 0);
        escreve_LCD(" "); // limpa o caractere do estado anterior
        break;

    case ESTADO1:
        *red_color += passo * inc_dec;
        OCR2A = *red_color; // PWM para o LED vermelho
        cmd_LCD(0xC4, 0);
        escreve_LCD("*");
        break;

    case ESTADO2:
        *green_color += passo * inc_dec;
        OCR1B = *green_color; // PWM para o LED verde
        cmd_LCD(0xC4, 0);
        escreve_LCD(" "); // limpa o caractere do estado anterior
        cmd_LCD(0xC9, 0);
        escreve_LCD("*");
        break;

    case ESTADO3:
        *blue_color += passo * inc_dec;
        OCR1A = *blue_color; // PWM para o LED azul
        cmd_LCD(0xCE, 0);
        escreve_LCD("*");
        cmd_LCD(0xC9, 0);
        escreve_LCD(" "); // limpa o caractere do estado anterior
        break;

    default:
        break;
    }
    _delay_ms(100);
}

ISR(PCINT1_vect)
{
    if (inc_dec == 0)
    {
        cont = 0;
        passo = 1;
    }

    if (PINC == 0x06)
    {
        //	S3 pressionado (+)
        inc_dec = 1;
    }
    else if (PINC == 0x0A)
    {
        // S2 pressionado (M)
        estado_mde += 1;
        if (estado_mde > ESTADO3)
        {
            estado_mde = ESTADO0;
        }
    }
    else if (PINC == 0x0C)
    {
        // S1 pressionado (-)
        inc_dec = -1;
    }
    else
    {
        inc_dec = 0; // limpa o incremento/decremento
    }
}

ISR(TIMER0_OVF_vect)
{
    cont++;
    if (cont >= 314)
    {
        // 5 segundos
        cont = 0;
        passo = 5;
    }
}
