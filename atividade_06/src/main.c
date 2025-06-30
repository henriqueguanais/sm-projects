#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "LCD.h"
#include "EEPROM.h"
#include "ADC.h"
#include "FIR.h"

#define ESTADO0 0
#define ESTADO1 1

volatile float inc_dec = 0; // -0.01 = decremento, 0.01 = incremento, 0 = nada
volatile uint8_t estado_mde = ESTADO0;
volatile uint8_t contador = 0;

void setup();
void mde(float *coefs);

float coefs[16] = {0.00977525, 0.01463677, 0.02838025, 0.04863182, 0.07189078, 0.0941352,
                         0.11151785, 0.12103207, 0.12103207, 0.11151785, 0.0941352, 0.07189078,
                         0.04863182, 0.02838025, 0.01463677, 0.00977525}; // Calculados para freq amostragem: 4000 Hz, fc = 3 Hz

int main()
{
    setup();
    ADC_init(); // inicializa o ADC

    // Lê os coeficientes da EEPROM
    // ler_vetores(coefs, 16);

    inic_LCD_4bits(); // inicializa o LCD

    uint8_t y = 0;

    while (1)
    {
        mde(coefs);
        salva_vetores(coefs, 16); // salva os coeficientes na EEPROM

        y = filtro_fir(coefs); // aplica o filtro FIR
        envia_dados(y);        // envia os dados para o DAC
    }
}

void setup()
{
    DDRC = 0x00;
    DDRC &= ~(1 << PC0); // entrada x
    DDRC &= ~0x0E;       // setando PC1 a PC3 como entrada, BTNS
    DDRD |= 0xFC;        // setando PD2, PD3 PD4 a PD7 como saida, LCD

    DDRC |= 0x30; // setando PC5 a PC6 como saida, coef (DA0, DA1)
    DDRB = 0x3F;  // setando PB0 a PB5 como saida, coef (DA2 - DA7)

    PCICR = 0x02; // interrupcao btn
    PCMSK1 = 0x0E;
    sei();
}

void mde(float *coefs)
{
    static char cont_str[2] = {0};   // string para armazenar o contador do coef atual
    static char coef_value[4] = {0}; // string para armazenar o valor do coeficiente
    
    switch (estado_mde)
    {
    case ESTADO0:
        cmd_LCD(0x80, 0);              // desloca cursor para a primeira linha
        escreve_LCD("   ELE-3717   "); // string armazenada na RAM
        cmd_LCD(0xC0, 0);              // desloca cursor para a segunda linha
        escreve_LCD("  FILTRO FIR  ");
        break;
    case ESTADO1:
        cmd_LCD(0x82, 0);
        escreve_LCD("Coeficiente");
        cmd_LCD(0xC0, 0); // desloca cursor para a segunda linha
        escreve_LCD("C");

        ident_num(contador, cont_str, 2); // converte o contador para string
        escreve_LCD(cont_str);            // escreve o contador no LCD
        cmd_LCD(0xC3, 0);
        escreve_LCD(":         ");

        cmd_LCD(0xC6, 0);
        uint8_t coef_int = (uint8_t)(coefs[contador]*1000); // converte o coeficiente atual para inteiro
        ident_num(coef_int, coef_value, 3); // converte o coeficiente atual para string
        escreve_LCD(coef_value);                   // escreve o coeficiente no LCD

        if (inc_dec != 0)
        {
            coefs[contador] += inc_dec; // incrementa ou decrementa o coeficiente
        }

        break;
    default:
        break;
    }
}

ISR(PCINT1_vect)
{
    if (PINC == 0x06)
    {
        //	S3 pressionado (M)
        if (estado_mde == ESTADO0)
        {
            estado_mde = ESTADO1; // muda para o próximo estado
        }
        else if (estado_mde == ESTADO1 && contador >= 15)
        {
            estado_mde = ESTADO0; // volta para o estado inicial
            contador = 0;         // reseta o contador
        }
        else
        {
            contador++; // incrementa o contador
        }
    }
    else if (PINC == 0x0A)
    {
        // S2 pressionado (+)
        inc_dec = 0.01; // seta incremento
    }
    else if (PINC == 0x0C)
    {
        // S1 pressionado (-)
        inc_dec = -0.01;
    }
    else
    {
        inc_dec = 0; // limpa o incremento/decremento
    }
}
