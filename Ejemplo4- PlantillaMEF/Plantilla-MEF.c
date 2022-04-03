// Tipo de datos para la variable de estados
typedef enum{
    ESTADO_INICIAL,
    ESTADO_1,
    ESTADO_2,
    ESTADO_N
} estadoMEF_t;

// Variable de estado (global)
estadoMEF_t estadoActual;

// Prototipos de funciones
void InicializarMEF(void );
void ActualizarMEF(void);

// Programa principal
int main (void){
	...
	InicializarMEF();
	...
	while(1){
		...
		ActualizarMEF();
		...
	}
	return 0;
}

// Función Inicializar MEF
void InicializarMEF(void){
	 estadoActual = ESTADO_INICIAL;
    // Resto de la inicializacion
}

// Función Actualizar MEF
void ActualizarMEF(void)
{
	switch (estadoActual) {
		case ESTADO_INICIAL:
        {
            // Actualizar salida del estado
    		// Chequear condiciones de transición de estado
			if(condicionesDeTransición == TRUE){
				// Cambiar a otro estado
				 estadoActual = ESTADO_N;
			}
		}
		break;
		case ESTADO_1:{
			...
		}
		break;
		case ESTADO_N:{
			...
		}
		break;
		default:{
			//Si cae en un estado no valido, reinicio
			InicializarMEF();
		}
		break;
	}	
}
