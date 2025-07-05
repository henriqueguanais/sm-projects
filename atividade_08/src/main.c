#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#include "LCD.h"
#include "gerador.h"
#include "FreeRTOS.h"
#include "task.h"

#define ESTADO0 0
#define ESTADO1 1
#define ESTADO2 2
#define ESTADO3 3

volatile int8_t inc_dec = 0; // -1 = decremento, 1 = incremento, 0 = nada
volatile uint8_t estado_mde = ESTADO0;
volatile uint8_t muda_parametro = 0; // flag para mudar o parametro
volatile uint8_t amp = 0;
volatile uint8_t offset = 0; // offset do sinal
volatile uint8_t freq = 1;	 // frequencia do sinal
volatile uint8_t cont = 0; // contador para o sinal senoide

void setup();
static void vtask_mde(void *pvParameters);
static void vtask_lcd(void *pvParameters);

int main(void)
{
	setup();
	inic_LCD_4bits();

	xTaskCreate(vtask_mde, (const char *)"mde", 256, NULL, 1, NULL);
	xTaskCreate(vtask_lcd, (const char *)"lcd", 256, NULL, 2, NULL);
	vTaskStartScheduler();
	for (;;);
}

void setup()
{
	DDRC = 0x00;
	DDRC = 0x00; // setando PC0 a PC3 como entrada, BTNS

	DDRD = 0x00;
	DDRD |= 0xFC; // setando PD2, PD3 PD4 a PD7 como saida, LCD

	DDRC |= 0x30; // setando PC5 a PC6 como saida, coef (DA0, DA1)
	DDRB = 0x3F;  // setando PB0 a PB5 como saida, coef (DA2 - DA7)

	PCICR = 0x0A; // interrupcao btn
	PCMSK1 = 0x0F;

	TCCR1A = 0x00; // modo ctc
	TCCR1B = 0x0C; // prescaler de 64
	TCNT1 = 0;
	TIMSK1 = (1 << OCIE1A); // HABILITA INTERRUPCAO COM COMPARADOR COM A
	OCR1A = (F_CPU/(2*freq/32)-1)/255;
	sei();
}

static void vtask_mde(void *pvParameters)
{
	for (;;)
	{
		switch (estado_mde)
		{
		case ESTADO0:
			OCR1A = (F_CPU/(2*freq)-1)/255;
			break;
		case ESTADO1:
			OCR1A = (F_CPU/(2*freq)-1)/65000;
			break;
		case ESTADO2:
			OCR1A = (F_CPU/(freq)-1)/65000;
			break;
		case ESTADO3:
			OCR1A = (F_CPU/(freq)-1)/8000;
			break;
		default:
			break;
		}

		freq += inc_dec; // incrementa ou decrementa a frequencia
		if (freq < 1)
			freq = 1; // limita a frequencia minima
		if (freq > 100)
			freq = 100; // limita a frequencia maxima

		vTaskDelay(10);
	}
}

static void vtask_lcd(void *pvParameters)
{
	static char freq_string[4] = {0};
	for (;;)
	{
		switch (estado_mde)
		{
		case ESTADO0:
			cmd_LCD(0x80, 0);		 // desloca cursor para a primeira linha
			escreve_LCD("T:QUA D:"); // string armazenada na RAM

			cmd_LCD(0x8A, 0);
			escreve_LCD("%");

			cmd_LCD(0x8D, 0);
			escreve_LCD("ON");

			cmd_LCD(0xC0, 0);
			ident_num(freq, freq_string, 3); // converte o contador para string
        	escreve_LCD(freq_string);

			cmd_LCD(0xC3, 0); // desloca cursor para a segunda linha
			escreve_LCD("Hz ");

			cmd_LCD(0xC9, 0);
			escreve_LCD("V");

			cmd_LCD(0xCE, 0);
			escreve_LCD("V");
			break;
		case ESTADO1:
			cmd_LCD(0x80, 0);		 // desloca cursor para a primeira linha
			escreve_LCD("T:TRI D:"); // string armazenada na RAM

			cmd_LCD(0x8A, 0);
			escreve_LCD("%");

			cmd_LCD(0x8D, 0);
			escreve_LCD("ON");

			cmd_LCD(0xC0, 0);
			ident_num(freq, freq_string, 3); // converte o contador para string
        	escreve_LCD(freq_string);

			cmd_LCD(0xC3, 0); // desloca cursor para a segunda linha
			escreve_LCD("Hz ");

			cmd_LCD(0xC9, 0);
			escreve_LCD("V");

			cmd_LCD(0xCE, 0);
			escreve_LCD("V");
			break;
		case ESTADO2:
			cmd_LCD(0x80, 0);	   // desloca cursor para a primeira linha
			escreve_LCD("T:RAM "); // string armazenada na RAM

			cmd_LCD(0x8D, 0);
			escreve_LCD("ON");

			cmd_LCD(0xC0, 0);
			ident_num(freq, freq_string, 3); // converte o contador para string
        	escreve_LCD(freq_string);

			cmd_LCD(0xC3, 0); // desloca cursor para a segunda linha
			escreve_LCD("Hz ");

			cmd_LCD(0xC9, 0);
			escreve_LCD("V");

			cmd_LCD(0xCE, 0);
			escreve_LCD("V");
			break;
		case ESTADO3:
			cmd_LCD(0x80, 0);	   // desloca cursor para a primeira linha
			escreve_LCD("T:SEN "); // string armazenada na RAM

			cmd_LCD(0x8D, 0);
			escreve_LCD("ON");

			cmd_LCD(0xC0, 0);
			ident_num(freq, freq_string, 3); // converte o contador para string
        	escreve_LCD(freq_string);

			cmd_LCD(0xC3, 0); // desloca cursor para a segunda linha
			escreve_LCD("Hz ");

			cmd_LCD(0xC9, 0);
			escreve_LCD("V");

			cmd_LCD(0xCE, 0);
			escreve_LCD("V");
			break;
		default:
			break;
		}
		vTaskDelay(20);
	}
}

ISR(PCINT1_vect)
{
	if ((PINC & 0x0F) == 0x07)
	{
		//	S3 pressionado (M)
		estado_mde++;
		if (estado_mde > ESTADO3)
		{
			estado_mde = ESTADO0; // volta ao estado inicial
		}
	}
	else if ((PINC & 0x0F) == 0x0E)
	{
		// S0 pressionado (+)
		inc_dec = 1; // seta incremento
	}
	else if ((PINC & 0x0F) == 0x0B)
	{
		// S2 pressionado (-)
		inc_dec = -1;
	}
	else if ((PINC & 0x0F) == 0x0D)
	{
		// S1 pressionado (muda parametro)
		muda_parametro += 1; // seta flag para mudar o parametro
		if (muda_parametro > 3)
		{
			muda_parametro = 0; // reseta a flag
		}
	}
	else
	{
		inc_dec = 0;		// limpa o incremento/decremento
		muda_parametro = 0; // limpa a flag de mudar parametro
	}
}

ISR(TIMER1_COMPA_vect)
{
	switch (estado_mde)
	{
	case ESTADO0:
		quadrada(255, 0);
		break;
	case ESTADO1:
		triangular(255, 0);
		break;
	case ESTADO2:
		rampa(255, 0);
		break;
	case ESTADO3:
		senoidal(255, 0, cont); // envia o sinal senoide
		break;
	default:
		break;
	}

	cont++; 
	if (cont >= 31)
	{
		cont = 0;
	}

	TCNT1 = 0; // reseta o contador do timer
}
