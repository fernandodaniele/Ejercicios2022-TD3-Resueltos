/*
 * pulsador.c
 *
 *  Created on: Jan 20, 2022
 *      Author: fer_e
 */

#include "pulsador.h"


#define T_ESPERA 5 //cantidad de veces que va a ingresar antes de cambiar de estado
estadoBoton_t estadoActual;

void inicializarBoton( GPIO_TypeDef  *puertoPulsador, uint16_t pinPulsador  )
{
    estadoActual = BAJO;  // Se configura estado inicial
}

void errorBoton( void )
{
   estadoActual = ALTO;
}

//Actualización de la MEF
void actualizarBoton( GPIO_TypeDef  *puertoPulsador, uint16_t pinPulsador  )
{
   static uint8_t contDescendente = 0;
   static uint8_t contAscendente = 0;

   switch( estadoActual ){

      case BAJO:
         if( HAL_GPIO_ReadPin(puertoPulsador, pinPulsador) ){
            estadoActual = ASCENDENTE;
         }
      break;

      case ASCENDENTE:
         if( contAscendente >= T_ESPERA ){
            if( HAL_GPIO_ReadPin(puertoPulsador, pinPulsador) ){
               estadoActual = ALTO;
               botonPresionado();
            } else{
               estadoActual = BAJO;
            }
            contAscendente = 0;
         }
         contAscendente++;
      break;

      case ALTO:
         if( !HAL_GPIO_ReadPin(puertoPulsador, pinPulsador) ){
            estadoActual = DESCENDENTE;
         }
      break;

      case DESCENDENTE:
         if( contDescendente >= T_ESPERA ){
            if( !HAL_GPIO_ReadPin(puertoPulsador, pinPulsador) ){
               estadoActual = BAJO;
               botonLiberado();
            } else{
               estadoActual = ALTO;
            }
            contDescendente = 0;
         }
         contDescendente++;
      break;

      default:
         errorBoton();
      break;
   }
}


void botonPresionado( void )
{
    //printf("Boton presionado\n");
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
}

void botonLiberado( void )
{
    //printf("Boton liberado\n");
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
}
