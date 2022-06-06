/*==================[ Inclusiones ]============================================*/
#include "pulsador.h"

/*==================[ Definiciones ]===================================*/

#define T_REBOTE_MS   40
#define T_REBOTE pdMS_TO_TICKS(T_REBOTE_MS)

/*==================[Prototipos de funciones]======================*/

static void errorPulsador( uint8_t  indice );
static void botonPresionado( uint8_t  indice );
static void botonLiberado( uint8_t  indice );

void tareaPulsador( void* taskParmPtr );

/*==================[Variables]==============================*/
uint16_t pinPulsador[N_PULSADOR] = { GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3 };
GPIO_TypeDef* puertoPulsador[N_PULSADOR] = { GPIOA,GPIOA,GPIOA,GPIOA };

pulsadorInfo pulsador [N_PULSADOR];

/*==================[Implementaciones]=================================*/
TickType_t obtenerDiferencia(uint8_t  indice)
{
    TickType_t tiempo;
    tiempo = pulsador[indice].diferenciaTiempo;
    return tiempo;
}

void borrarDiferencia( uint8_t  indice )
{
    pulsador[indice].diferenciaTiempo = TIEMPO_NO_VALIDO;
}

void inicializarPulsador( void )
{
	for(int indice = 0; indice < N_PULSADOR; indice++)
	{
		pulsador [indice].tecla             = pinPulsador[indice];
		pulsador [indice].puerto            = puertoPulsador[indice];
		pulsador [indice].estado            = BAJO;                     //Estado inicial
		pulsador [indice].tiempoBajo        = TIEMPO_NO_VALIDO;
		pulsador [indice].tiempoAlto        = TIEMPO_NO_VALIDO;
		pulsador [indice].diferenciaTiempo  = TIEMPO_NO_VALIDO;
	}
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

static void errorPulsador( uint8_t  indice )
{
    pulsador[indice].estado = ALTO;
}

// pulsador_ Update State Function
void actualizarPulsador( uint8_t  indice)
{
    switch( pulsador [indice].estado )
    {
    	case BAJO:
             if( HAL_GPIO_ReadPin(puertoPulsador[indice], pinPulsador[indice]) ){
                pulsador [indice].estado = ASCENDENTE;
             }
          break;

          case ASCENDENTE:
                if( HAL_GPIO_ReadPin(puertoPulsador[indice], pinPulsador[indice]) ){
                   pulsador [indice].estado = ALTO;
                   botonPresionado(indice);
                } else{
                   pulsador [indice].estado = BAJO;
                }
          break;

          case ALTO:
             if( !HAL_GPIO_ReadPin(puertoPulsador[indice], pinPulsador[indice]) ){
                pulsador [indice].estado = DESCENDENTE;
             }
          break;

          case DESCENDENTE:
                if( !HAL_GPIO_ReadPin(puertoPulsador[indice], pinPulsador[indice]) ){
                   pulsador [indice].estado = BAJO;
                   botonLiberado(indice);
                } else{
                   pulsador [indice].estado = ALTO;
                }
          break;

          default:
        	  errorPulsador(indice);
          break;
    }
}

/* accion de el evento de tecla pulsada */
static void botonPresionado( uint8_t  indice)
{
    TickType_t conteoTicksActuales = xTaskGetTickCount();   //Medimos el tiempo en ticks desde que inició el scheduler
    pulsador [indice].tiempoBajo = conteoTicksActuales;             //guardamos ese tiempo como referencia
}

/* accion de el evento de tecla liberada */
static void botonLiberado( uint8_t  indice)
{
    TickType_t conteoTicksActuales = xTaskGetTickCount();   //Medimos el tiempo en ticks desde que inició el scheduler
    pulsador [indice].tiempoAlto    = conteoTicksActuales;
    pulsador [indice].diferenciaTiempo  = pulsador [indice].tiempoAlto - pulsador [indice].tiempoBajo; //Da el tiempo que el pulsador estuvo en estado alto
}

void tareaPulsador( void* taskParmPtr )
{
    while( 1 )
    {
    	for (int i = 0; i < N_PULSADOR; i++)
		{
			actualizarPulsador(i);
		}
		vTaskDelay( T_REBOTE );
    }
}
