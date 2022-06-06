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
#define TIEMPO_NO_VALIDO   	0
#define N_PULSADOR     		4                           //Cantidad de pulsadores

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
    GPIO_TypeDef* puerto;
} pulsadorInfo;

/*==================[Prototipos de funciones]======================*/
void inicializarPulsador(  void );
TickType_t obtenerDiferencia( uint8_t  indice);
void borrarDiferencia( uint8_t  indice );

#endif /* INC_PULSADOR_H_ */
