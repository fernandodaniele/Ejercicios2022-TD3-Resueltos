/*==================[ Inclusiones ]============================================*/
#include "pulsador.h"

/*==================[ Definiciones ]===================================*/

#define T_REBOTE_MS   40
#define T_REBOTE pdMS_TO_TICKS(T_REBOTE_MS)

/*==================[Prototipos de funciones]======================*/

static void errorPulsador( void );
static void botonPresionado( void );
static void botonLiberado( void );

void tareaPulsador( void* taskParmPtr );

/*==================[Variables]==============================*/
uint16_t pinPulsador[1] = { B1_Pin };
GPIO_TypeDef* puertoPulsador[1] = { GPIOA };

pulsadorInfo pulsadorA;

/*==================[Implementaciones]=================================*/
TickType_t obtenerDiferencia()
{
    TickType_t tiempo;
    tiempo = pulsadorA.diferenciaTiempo;
    return tiempo;
}

void borrarDiferencia( void )
{
    pulsadorA.diferenciaTiempo = TIEMPO_NO_VALIDO;
}

void inicializarPulsador( void )
{
    pulsadorA.tecla             = pinPulsador[0];
    pulsadorA.estado            = BAJO;                     //Estado inicial
    pulsadorA.tiempoBajo        = TIEMPO_NO_VALIDO;
    pulsadorA.tiempoAlto        = TIEMPO_NO_VALIDO;
    pulsadorA.diferenciaTiempo  = TIEMPO_NO_VALIDO;

    // Crear tareas en freeRTOS
    BaseType_t res = xTaskCreate(
    	tareaPulsador,                     	// Funcion de la tarea a ejecutar
        "tareaPulsador",   	                // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 		// Cantidad de stack de la tarea
        NULL,                          	// Parametros de tarea
        tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL                          		// Puntero a la tarea creada en el sistema
    );

    // Gestion de errores
	if(res == pdFAIL)
	{
		while(1);					// si no pudo crear la tarea queda en un bucle infinito
	}
}

static void errorPulsador( void )
{
    pulsadorA.estado = ALTO;
}

// pulsador_ Update State Function
void actualizarPulsador()
{
    switch( pulsadorA.estado )
    {
    	case BAJO:
             if( HAL_GPIO_ReadPin(puertoPulsador[0], pinPulsador[0]) ){
                pulsadorA.estado = ASCENDENTE;
             }
          break;

          case ASCENDENTE:
                if( HAL_GPIO_ReadPin(puertoPulsador[0], pinPulsador[0]) ){
                   pulsadorA.estado = ALTO;
                   botonPresionado();
                } else{
                   pulsadorA.estado = BAJO;
                }
          break;

          case ALTO:
             if( !HAL_GPIO_ReadPin(puertoPulsador[0], pinPulsador[0]) ){
                pulsadorA.estado = DESCENDENTE;
             }
          break;

          case DESCENDENTE:
                if( !HAL_GPIO_ReadPin(puertoPulsador[0], pinPulsador[0]) ){
                   pulsadorA.estado = BAJO;
                   botonLiberado();
                } else{
                   pulsadorA.estado = ALTO;
                }
          break;

          default:
        	  errorPulsador();
          break;
    }
}

/* accion de el evento de tecla pulsada */
static void botonPresionado()
{
    TickType_t conteoTicksActuales = xTaskGetTickCount();   //Medimos el tiempo en ticks desde que inició el scheduler
    HAL_GPIO_WritePin(GPIOD, LD4_Pin,1);          //para tener una referencia en el debug
    pulsadorA.tiempoBajo = conteoTicksActuales;             //guardamos ese tiempo como referencia
}

/* accion de el evento de tecla liberada */
static void botonLiberado()
{
    TickType_t conteoTicksActuales = xTaskGetTickCount();   //Medimos el tiempo en ticks desde que inició el scheduler
    HAL_GPIO_WritePin(GPIOD, LD4_Pin,0);         //para tener una referencia en el debug
    pulsadorA.tiempoAlto    = conteoTicksActuales;
    pulsadorA.diferenciaTiempo  = pulsadorA.tiempoAlto - pulsadorA.tiempoBajo; //Da el tiempo que el pulsador estuvo en estado alto
}

void tareaPulsador( void* taskParmPtr )
{
    while( 1 )
    {
        actualizarPulsador();
        vTaskDelay( T_REBOTE );
    }
}
