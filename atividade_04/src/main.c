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
volatile uint8_t valor_atual = 0;

void setup();
void mde(uint8_t red_color, uint8_t green_color, uint8_t blue_color);


int main()
{
    setup();

    uint8_t red_color = 0;
    uint8_t green_color = 0;
    uint8_t blue_color = 0;

    inic_LCD_4bits(); //inicializa o LCD
    escreve_LCD(" RED GREEN BLUE"); //string armazenada na RAM
    cmd_LCD(0xC0,0); //desloca cursor para a segunda linha
    
    escreve_LCD("   0    0    0"); //string armazenada na RAM

    while(1) {
        cmd_LCD(0x02, 0);       // retorna cursor para o início da linha

        cmd_LCD(0xC3, 0);       // unidade red
        escreve_LCD((char *)&red_color);
        cmd_LCD(0xC8, 0);       // unidade green
        escreve_LCD((char *)&green_color);
        cmd_LCD(0xCD, 0);       // unidade blue
        escreve_LCD((char *)&blue_color);

        mde(red_color, green_color, blue_color);
    }
}

void setup()
{
	DDRB |= 0x0E;// setando PB1 a PB3 como saida, LEDS
	DDRC &= ~0x0E;// setando PC1 a PC3 como entrada, BTNS
	DDRD |= 0xFC;// setando PD2, PD3 PD4 a PD7 como saida, LCD
	
	PCICR = 0x02;//interrupcao btn
	PCMSK1 = 0x0E;
	sei();
	
}

void mde(uint8_t red_color, uint8_t green_color, uint8_t blue_color)
{
	switch(estado_mde){
		case ESTADO0:
        cmd_LCD(0xCE, 0);
        escreve_LCD(" ");
			break;
		case ESTADO1:
        valor_atual = red_color;
        cmd_LCD(0xC4, 0);       
        escreve_LCD("*");
			break;
		case ESTADO2:
        valor_atual = green_color;
        cmd_LCD(0xC4, 0);       
        escreve_LCD(" ");
        cmd_LCD(0xC9, 0);
        escreve_LCD("*");
			break;
		case ESTADO3:
        valor_atual = blue_color;
        cmd_LCD(0xCE, 0);
        escreve_LCD("*");
        cmd_LCD(0xC9, 0);
        escreve_LCD(" ");
			break;
		default:
			break;
	}
}

void pwm_led() {

}

ISR(PCINT1_vect)
{
    
	if (PINC==0x06){ 
		//	S3 pressionado (+)
        valor_atual += 1;
        if(valor_atual > 255){
            valor_atual = 255;
        }
        

	}
	else if(PINC==0x0A){
		// S2 pressionado (M)
        estado_mde += 1;
        if(estado_mde > ESTADO3){
            estado_mde = ESTADO0;
        }
	}
	else if(PINC==0x0C){
		// S1 pressionado (-)
        valor_atual -= 1;
        if(valor_atual < 0){
            valor_atual = 0;
        }
	}
	
}
