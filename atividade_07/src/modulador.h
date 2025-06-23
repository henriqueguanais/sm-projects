#ifndef MODULADOR_H
#define MODULADOR_H

uint8_t modula_am(uint16_t freq_portadora, uint16_t msg);
uint8_t modula_fm(uint16_t freq_portadora, uint16_t msg);
uint8_t modula_ask(uint16_t freq_portadora, uint16_t msg);
uint8_t modula_fsk(uint16_t freq_portadora, uint16_t msg);
void envia_dados(uint8_t y);

#endif