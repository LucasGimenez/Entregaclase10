/*============================================================================
 * Licencia: 
 * Autor:   Vegh Juan Francisco Llamazares, Lucas Andres Gimenez, Fernando Guardia, Carlos Eduardo López Aldana.
 * Fecha:   21/04/2017
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "sapi.h"                     // <= Biblioteca sAPI
#include "MEF_tecladoMatricial.h"   // <= su propio archivo de cabecera

/*==================[definiciones y macros]==================================*/

typedef enum{
    EN_ESPERA_DE_DIGITO_1,
	EN_ESPERA_DE_DIGITO_2,
	EN_ESPERA_DE_LETRA,
	GUARDAR_PISO
	
} mefTecladoMatricial_t;

typedef enum{
   ESCANEANDO_TECLADO,
   APRETANDO_TECLA,
   IDENTIFICAR_TECLA_Y_ESCRIBIR
} mefScanTeclado_t;


/*==================[definiciones de datos internos]=========================*/
/*==================[definiciones de datos externos]=========================*/

uint16_t primerDigito  = 0; // Variable para almacenar el primer digito ingresado.
uint16_t segundoDigito = 0; // Variable para almacenar el segundo digito ingresado.
uint16_t confirmar = 0;     // Variable para almacenar si se confirma o cancela en ingreso de los datos.

uint32_t indiceTeclaPresionada = 0; // Variable que me permite saber cuantas veces fue presionado el teclado y asi saber que tecla se espera.
uint32_t indiceTeclaGuardar = 0;    // Variable que me permite conocer que digito fue ingresado para su posterior almacenamiento. (Similar a la anterior pero se usa en main).


int almacenarPisos [10];    // Vector para almacenar hasta 10 pisos ingresados correctamente.
int indice = 0;             // Indice para recorrer el vector anterior.

// Guarda la ultima tecla apretada
uint16_t key = 0;


extern bool_t flagConfiguracion;

mefTecladoMatricial_t estadoMefTecladoMatricial;
bool_t hayTeclaDisponible = 0;
uint16_t teclaPresionada = 0xff;

// Variable de estado de la MEF captura estodo de tecla.
mefScanTeclado_t estadoMefScanTeclado;

/* Vector para almacenar los pines que van a estar conectados a las filas del teclado matricial. */
uint8_t pinesFila[4] = {
    RS232_TXD,
    CAN_RD,
    CAN_TD,
    T_COL1
};

/* Vector para almacenar los pines que van a estar conectados a las columnas del teclado matricial. */
uint8_t pinesColumna[4] = {
    T_FIL0,
    T_FIL3,
    T_FIL2,
    T_COL0
};

static uint16_t pinesTeclado[16] = {
                                1, 2, 3, 'A',
                                4, 5, 6, 'B',
                                7, 8, 9, 'C',
                                '*', 0, '#', 'D'
};


/*==================[declaraciones de funciones internas]====================*/

//*********************************************************************************************************************
//
//*********************************************************************************************************************
void configurarTecladoMatricial (void){
    
uint8_t i = 0;  // Variable para recorrer el vector de filas y columnas.
    
/* Se recorre todo el vector de filas para configurar los pines como salida.*/
    for ( i = 0; i < 4; i++){
        gpioConfig(pinesFila[i], GPIO_OUTPUT);
    } 

/* Se recorre todo el vector de columnas para configurar los pines como entrada en PULLUP (resistencia interna).*/
    for ( i = 0; i < 4; i++){
        gpioConfig(pinesColumna[i], GPIO_INPUT_PULLUP);
    }
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
void inicializarMEF_tecladoMatrical(void) {

estadoMefTecladoMatricial = EN_ESPERA_DE_DIGITO_1;
configurarTecladoMatricial();
estadoMefScanTeclado = ESCANEANDO_TECLADO;

}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
bool_t ScanTeclas(void)
{
// Funcion devuelve 1 si hay por lo menos una presionada.
// Carga en "key" el indice de la tecla presionada.
	
bool_t ret = 0;

uint8_t f = 0; // Variable para recorrer los vectores de filas.
uint8_t c = 0; // Variable para recorrer los vectores de columnas.

// Lo primero que hacemos es poner todas las filas en estado "bajo".
for (f=0; f<4; f++)
	gpioWrite(pinesFila[f], 0);
for (c=0; c<4; c++)
	{
	// Si leo un estado BAJO en una columna entonces puede haber una tecla presionada-
        if (!gpioRead(pinesColumna[c]))
		{
		//Se ponen todas las filas en "alto", excepto la primera que se deja en "bajo".
		for (f=1; f<4 ; f++)
			gpioWrite(pinesFila[f], 1);
		// Se procede a buscar la tecla presionada, para ello se recorren las filas. Acá lo que buscamos es la coordenada de la fila.
		for (f=0; f<4; f++)
			{
			// El siguiente if es para evitar un índice negativo en el vector.
			if (f > 0)
				gpioWrite(pinesFila[f-1], 1);
			gpioWrite(pinesFila[f], 0);

			//Se testea la columna actual con la fila actual, si detectamos un estado "BAJO"
			//en alguna columna, significa que hemos encontrado la coordenada de la fila 
			//correspondiente. Con esta coordenada ya podemos detectar que tecla fue presionada.
			if (!gpioRead(pinesColumna[c]))
				{
				ret = 1;
				key = f * 4 + c;
				return ret;
				/*
				  Formula de teclas para Teclado Matricial (Keypad)
				  de 4 filas (rows) * 4 columnas (columns)

				      c0 c1 c2 c3    Si se presiona la tecla row[f] column[c]:
				  f0  0  1  2  3    
				  f1  4  5  6  7    valor = (f) * cantidadDeColumnas + (c)
				  f2  8  9 10 11    
				  f3 12 13 14 15 
				*/
				}
			}
		}
	}
return ret;
}
//*********************************************************************************************************************
//*********************************************************************************************************************



//*********************************************************************************************************************
//
//*********************************************************************************************************************
void ActualizaMEFScanTecla(void)
{
// Esta rutina hace la captura de una tecla con anti-bouncing.
// Al haber confirmado el proceso de una tecla pone "hayTeclaDisponible" en 1;
// Y la tecla confirmada en "teclaPresionada".

static delay_t delayAntirebote;
static bool_t flagEstadoApretandoTecla = 0;


switch(estadoMefScanTeclado)
	{
	case ESCANEANDO_TECLADO:
		// Chequear condiciones de transicion de estado.
		// Hay una tecla presionada?
		if (ScanTeclas())
			{
			teclaPresionada = key;
			estadoMefScanTeclado = APRETANDO_TECLA;
			}
	break;


	case APRETANDO_TECLA:
		// Con un flag ejecuto una vez al ingresar al estado.
		if (flagEstadoApretandoTecla == 0)
			{
			flagEstadoApretandoTecla= 1;
			delayConfig(&delayAntirebote, 40);
			}
		// Salida en el estado.
		// El delay se incrementa solo!
		// Chequear condiciones de transicion de estado.
		if (delayRead(&delayAntirebote))
			{
			ScanTeclas();
			// Esta la misma tecla presionada?
			if (teclaPresionada == key)
				{
				estadoMefScanTeclado = IDENTIFICAR_TECLA_Y_ESCRIBIR;
				flagEstadoApretandoTecla = 0;
				}
			// No esta presionada la misma tecla.
			else	{
				estadoMefScanTeclado = ESCANEANDO_TECLADO;
				teclaPresionada = 0xff;
				flagEstadoApretandoTecla = 0;
				}
			}      


	break;


	case IDENTIFICAR_TECLA_Y_ESCRIBIR:
		// Se solto la tecla?
		if (!(ScanTeclas()))
			{
			estadoMefScanTeclado = ESCANEANDO_TECLADO;  
			hayTeclaDisponible = 1;
			}
	break;

	
	default:
	inicializarMEF_tecladoMatrical();
	break;
   }
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
void actualizarMEF_tecladoMatricial (void) { 

ActualizaMEFScanTecla();
	
switch (estadoMefTecladoMatricial) {

	case EN_ESPERA_DE_DIGITO_1:
		if( ingresarDigito() ) {
				estadoMefTecladoMatricial = EN_ESPERA_DE_DIGITO_2;
		}
	break;
		
	case EN_ESPERA_DE_DIGITO_2:
			if( ingresarDigito() ) {
					estadoMefTecladoMatricial = EN_ESPERA_DE_LETRA;
				

				if (pinesTeclado[segundoDigito] == 'A') {
					guardarPisoSimple (primerDigito);
					estadoMefTecladoMatricial = EN_ESPERA_DE_DIGITO_1;

				}
				else if (pinesTeclado[confirmar] == 'B') {
					estadoMefTecladoMatricial = EN_ESPERA_DE_DIGITO_1;
					cancelar();
				}
			}
		break;

	case EN_ESPERA_DE_LETRA:
			if ( ingresarDigito() ) {
					estadoMefTecladoMatricial = EN_ESPERA_DE_LETRA;
				
				if (pinesTeclado[confirmar] == 'A' ) {
					estadoMefTecladoMatricial = GUARDAR_PISO;
					guardarPisoDoble (primerDigito, segundoDigito);
				}

				else if (pinesTeclado[confirmar] == 'B') {
					estadoMefTecladoMatricial = EN_ESPERA_DE_DIGITO_1;
					cancelar();
				}
			}
		break;

	default:	
		inicializarMEF_tecladoMatrical();
	break;
	}		
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
bool_t ingresarDigito (void)
{
bool_t ret = FALSE;	

/* Se testea la columna actual con la fila actual, si detectamos un estado "BAJO"
    en alguna columna, significa que hemos encontrado la coordenada de la fila 
    correspondiente. Con esta coordenada ya podemos detectar que tecla fue presionada.*/
if (hayTeclaDisponible)
	{
	hayTeclaDisponible = 0;
	ret = TRUE;
                
	/* Etapa en la cual se evalua que digito se ingreso, basicamente llevando la cuenta de las veces que se presionó una tecla*/
	switch (indiceTeclaPresionada)
		{
		case 0:  primerDigito = teclaPresionada;
			   indiceTeclaPresionada = 1;
		break;

		case 1:  segundoDigito = teclaPresionada;
			    if ((pinesTeclado[segundoDigito] == 'A') || (pinesTeclado[segundoDigito] == 'B')) {
				indiceTeclaPresionada = 0;
				indiceTeclaGuardar = 0;
			    }
			    else {
				indiceTeclaPresionada = 2;
			    }
		break;
		    
		case 2:  confirmar = teclaPresionada;
			   indiceTeclaPresionada= 0;
			   indiceTeclaGuardar = 1;
		break;
		}
	}
return ret;
}    
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
void guardarPisoSimple (int primerDigito) {
	/*======= Funcion que almacena el piso ingresado, de un solo digito, en el vector =======*/
	if (indice < 10) {

		if (indice > 0)
			almacenarPisos[indice] = pinesTeclado[primerDigito];
		else
			almacenarPisos[0] = pinesTeclado[primerDigito];

		indice ++;
	}

	/*======= Etapa para reinicializar las variables utilizadas y prepararlas para el proximo ingreso =======*/
	primerDigito  = 0;
	segundoDigito = 0;
	indiceTeclaGuardar = 0;
}

void guardarPisoDoble (int primerDigito, int segundoDigito) {
	
	/*======= Funcion que almacena el piso ingresado, de dos digitos, en el vector =======*/
	
	int carga = 0;
	
	/*======= Etapa de guardar un subsuelo (numero negativo) en el vector de almacenamiento =======*/
	if (pinesTeclado[primerDigito] == '#' && pinesTeclado[segundoDigito] <= 5)
		carga = 0 - pinesTeclado[segundoDigito];
	else 
		carga = (pinesTeclado[primerDigito] * 10) + pinesTeclado[segundoDigito];
	
	/*======= Etapa para verificar si se desea entrar al modo configuracion =======*/
    if (carga == 99) {
    flagConfiguracion = TRUE;
    }
	
	/*======= Etapa de carga en el vector que almacena los pisos ingresados =======*/	
	if (indice < 10){

		if (carga <= 20){

			if (indice > 0)
				almacenarPisos[indice] = carga;
			else 
				almacenarPisos[0] = carga;

			indice++;
		}
	}
    
	/*======= Etapa para reinicializar las variables utilizadas y prepararlas para el proximo ingreso =======*/
	primerDigito  = 0;
	segundoDigito = 0;
	indiceTeclaGuardar = 0;

}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
void cancelar (void) {
    
	/*======= Para cancelar la operacion le seteamos un valor no valido a las siguientes variables =======*/
	uint16_t primerDigito  = 0; 
	uint16_t segundoDigito = 0; 
	uint16_t confirmar = 0;     

	uint32_t indiceTeclaPresionada = 0; 
	uint32_t indiceTeclaGuardar = 0;
    
}
//*********************************************************************************************************************
//*********************************************************************************************************************




/*==================[fin del archivo]========================================*/
