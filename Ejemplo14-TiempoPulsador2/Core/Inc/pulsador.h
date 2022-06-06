/*
 * pulsador.h
 *
 *  Created on: May 30, 2022
 *      Author: fer_e
 */

#ifndef INC_PULSADOR_H_
#define INC_PULSADOR_H_

/*==================[ Inclusiones ]============================================*/
#include "FreeRTOS.h"
#include "task.h"
#include "gpio.h"
/*==================[ Definiciones ]===================================*/
#define TIEMPO_NO_VALIDO   0

typedef enum
{
    ALTO,
    BAJO,
    DESCENDENTE,
    ASCENDENTE
} estadoPulsador;

typedef struct
{
	uint16_t tecla;
    estadoPulsador estado;   //variables
    TickType_t tiempoBajo;		//tiempo de la última transición del estado alto a bajo
    TickType_t tiempoAlto;		    //tiempo de la última transición del estado bajo a alto
    TickType_t diferenciaTiempo;	    //variables
} pulsadorInfo;

/*==================[Prototipos de funciones]======================*/
void inicializarPulsador( void );
TickType_t obtenerDiferencia( void);
void borrarDiferencia( void );


#endif /* INC_PULSADOR_H_ */
