/*
 * pulsador.h
 *
 *  Created on: Jan 20, 2022
 *      Author: fer_e
 */

#ifndef INC_PULSADOR_H_
#define INC_PULSADOR_H_

#include "gpio.h"

typedef enum{
   ALTO,
   BAJO,
   DESCENDENTE,
   ASCENDENTE
} estadoBoton_t;

void errorBoton( void );
void inicializarBoton( GPIO_TypeDef  *puertoPulsador, uint16_t pinPulsador );
void actualizarBoton( GPIO_TypeDef  *puertoPulsador, uint16_t pinPulsador );
void botonPresionado( void );
void botonLiberado( void );

#endif /* INC_PULSADOR_H_ */
