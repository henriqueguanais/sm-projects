#ifndef GERADOR_H
#define GERADOR_H

void envia_dados(uint16_t y);
void quadrada(uint8_t amp, uint8_t offset);
void triangular(uint8_t amp, uint8_t offset);
void senoidal(uint8_t amp, uint8_t offset, uint8_t index);
void rampa(uint8_t amp, uint8_t offset);

#endif 