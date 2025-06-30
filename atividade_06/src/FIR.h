#ifndef FIR_H
#define FIR_H

#include <avr/io.h>

uint8_t filtro_fir(const float *coefs);
void envia_dados(uint8_t y);

#endif