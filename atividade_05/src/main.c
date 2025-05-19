#include <avr/io.h> 
#include <util/delay.h> 
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "LCD.h"
#include "joystick.h"

#define ESTADO0 0
#define ESTADO1 1
#define ESTADO2 2
#define ESTADO3 3
#define ESTADO4 4

volatile uint8_t estado_mde = ESTADO0;
volatile int8_t passo = 1;                         // passo de incremento, decremento
volatile int8_t inc_dec = 0;                        // -1 = decremento, 1 = incremento, 0 = nada
volatile uint16_t cont = 0;

void setup();
void mde();

int main()
{
    setup();

    uint8_t pos_x[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t pos_y[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    char pos_x_str[2];
    char pos_y_str[2];

    inic_LCD_4bits(); //inicializa o LCD
    escreve_LCD("X:"); //string armazenada na RAM
    cmd_LCD(0xC0,0); //desloca cursor para a segunda linha
    escreve_LCD("Y:"); //string armazenada na RAM
    cmd_LCD(0x83, 0);   // linha x
    for (int i=0; i<8; i++) {
        ident_num(pos_x[i], pos_x_str);
        escreve_LCD(pos_x_str);
    }
    //escreve_LCD("0 0 0 0 0 0 0");
    cmd_LCD(0xC3, 0);   // linha y
    for (int i=0; i<8; i++) {
        ident_num(pos_y[i], pos_y_str);
        escreve_LCD(pos_y_str);
    }
    //escreve_LCD("0 0 0 0 0 0 0");

    while(1) {
        cmd_LCD(0x02, 0);       // retorna cursor para o início da linha

        mde();
    }
    
}

void setup()
{
	DDRC &= ~0x30;// setando PC4 e PC5 como entrada, joystick
	DDRD |= 0xFC;// setando PD2, PD3 PD4 a PD7 como saida, LCD
    DDRB |= 0x2D;     // setando PB2, PB3, PB5, MAX7219 como saida
	
    TCCR0A = 0x00;     // timer de 16 ms                  
	TCCR0B = 0xC5;       // prescaler de 1024               
	TCNT0  = 0;                         
	TIMSK0 = (1<<TOIE0); 
	sei();


}


void mde()
{
	switch(estado_mde) {

		case ESTADO0:
   
			break;

		case ESTADO1:

			break;

		case ESTADO2:

			break;

		case ESTADO3:

			break;

        case ESTADO4:

            break;
		default:
			break;
	}
    _delay_ms(100);
}


ISR(TIMER0_OVF_vect)
{
    cont ++;
    if (cont >= 314) {
        // 5 segundos
        cont = 0;
        passo = 5;
    }
}
