#ifndef LED_H //chequea que otro archivo no haya llamado a led.h
#define LED_H

#include "gpio.h"

void configurarLed(uint16_t ledR, uint16_t ledA, uint16_t ledV); //especifica en que pines conectar cada led: ROJO, AMARILLO, VERDE

void prenderLed(char led);

void apagarLed(char led);

#endif
