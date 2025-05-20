#ifndef JOYSTICK_H
#define JOYSTICK_H

#define LIM_INF_DZ 200  // limite inferior da deadzone
#define LIM_SUP_DZ 800  // limite superior da deadzone

void setup_joystick();
void ler_joystick(uint16_t *valor_y, uint16_t *valor_x);
void joystick_direction(uint16_t valor_y, uint16_t valor_x, uint8_t *direction_vector); 

#endif