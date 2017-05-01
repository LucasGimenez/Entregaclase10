/*============================================================================
 * Licencia: 
 * Autor:   Vegh Juan Francisco Llamazares, Lucas Andres Gimenez, Fernando Guardia, Carlos Eduardo López Aldana.
 * Fecha:   21/04/2017
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "sapi.h"                     // <= Biblioteca sAPI
#include "MEF_tecladoMatricial.h"   // <= su propio archivo de cabecera

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/
// SACAR PARA PRUEBAS...
extern uint32_t pasos;

/*==================[definiciones de datos externos]=========================*/


uint16_t primerDigito  = 0; // Variable para almacenar el primer digito ingresado.
uint16_t segundoDigito = 0; // Variable para almacenar el segundo digito ingresado.
uint16_t confirmar = 0;     // Variable para almacenar si se confirma o cancela en ingreso de los datos.

uint32_t indiceTeclaPresionada = 0; // Variable que me permite saber cuantas veces fue presionado el teclado y asi saber que tecla se espera.
uint32_t indiceTeclaGuardar = 0;    // Variable que me permite conocer que digito fue ingresado para su posterior almacenamiento. (Similar a la anterior pero se usa en main).


int almacenarPisos [10] = {0};    // Vector para almacenar hasta 10 pisos ingresados correctamente.
int indice = 0;                  // Indice para recorrer el vector anterior.

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

delay_t miDelay;


/*==================[declaraciones de funciones internas]====================*/


//*********************************************************************************************************************
//
//*********************************************************************************************************************
void actualizarMEF_tecladoMatricial (void) { 

       if (ingresarDigito()) {
	
            delay(500);
            
            switch (indiceTeclaGuardar){
                
                case 0:  if (pinesTeclado[segundoDigito] == 'A') {
                                guardarPisoSimple (primerDigito);
				
                            }
                            else if (pinesTeclado[confirmar] == 'B') {
                                cancelar();
                            }
                        break;
                            
                case 1:   if (pinesTeclado[confirmar] == 'A' ) {
                                guardarPisoDoble (primerDigito, segundoDigito);
                            }
                
                            else if (pinesTeclado[confirmar] == 'B') {
                                cancelar();
                            }
                        break;
            }
        }
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
void configurarTeclado (void){
    
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
bool_t ingresarDigito (void){
    
    bool_t ret = FALSE;
    
    uint16_t i = 0; // Variable para recorrer los vectores de filas.
    uint16_t j = 0; // Variable para recorrer los vectores de columnas.
    
/* Lo primero que hacemos es poner todas las filas en estado "bajo".*/
    for ( i = 0; i < 4; i++){
        gpioWrite (pinesFila[i], LOW);
        }
        
/* Lo siguiente es recorrer las columnas.*/
    for ( j = 0; j < 4; j++) {
        
        if( !gpioRead( pinesColumna[j] ) ) {
            
            delay(50);  // Delay anti-rebote.
    
/* Se ponen todas las filas en "alto", excepto la primera que se deja en "bajo".*/
            for( i = 1; i < 4; i++) {
            gpioWrite (pinesFila[i], HIGH);
            }
        }
            
/* Se procede a buscar la tecla presionada, para ello se recorren las filas. Acá lo que buscamos es la coordenada de la fila.*/
        for ( i = 0; i < 4; i++){
   
            
/* El siguiente if es para evitar un índice negativo en el vector. */
            if( i > 0){
                gpioWrite (pinesFila[i-1], HIGH);
            }
            
            gpioWrite (pinesFila[i], LOW);

/* Se testea la columna actual con la fila actual, si detectamos un estado "BAJO"
    en alguna columna, significa que hemos encontrado la coordenada de la fila 
    correspondiente. Con esta coordenada ya podemos detectar que tecla fue presionada.*/
            if( !gpioRead( pinesColumna[j] ) ){
               ret = TRUE;
                
/* Etapa en la cual se evalua que digito se ingreso, basicamente llevando la cuenta de las veces que se presionó una tecla*/
                switch (indiceTeclaPresionada){
                    
                case 0:  primerDigito = i * 4 + j;
                           indiceTeclaPresionada = 1;
                        break;
                
                case 1:  segundoDigito = i * 4 + j;
                            if ((pinesTeclado[segundoDigito] == 'A') || (pinesTeclado[segundoDigito] == 'B')) {
                                indiceTeclaPresionada = 0;
                                indiceTeclaGuardar = 0;
                            }
                            else {
                                indiceTeclaPresionada = 2;
                            }
                        break;
                    
                case 2:  confirmar = i * 4 + j;
                           indiceTeclaPresionada= 0;
                           indiceTeclaGuardar = 1;
                        break;
                }
                          
                /*
                  Formula de teclas para Teclado Matricial (Keypad)
                  de 4 filas (rows) * 4 columnas (columns)

                      c0 c1 c2 c3    Si se presiona la tecla r[i] c[j]:
                  r0  0  1  2  3    
                  r1  4  5  6  7    valor = (i) * cantidadDeColumnas + (j)
                  r2  8  9 10 11    
                  r3 12 13 14 15 
               */
        
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
void guardarPisoSimple (int primerDigito)
{
/*======= Funcion que almacena el piso ingresado, de un solo digito, en el vector =======*/
if (indice < 10)
	{
	if (indice > 0)
		almacenarPisos[indice] = pinesTeclado[primerDigito];
	else	almacenarPisos[0] = pinesTeclado[primerDigito];
	indice ++;
	}
primerDigito  = 0;
segundoDigito = 0;
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
void guardarPisoDoble (int primerDigito, int segundoDigito)
{
if (indice < 10)
	{
	/*======= Funcion que almacena el piso ingresado, de dos digitos, en el vector =======*/
	int carga = 0;
	/*======= Etapa de almacenar en la variable 'carga' el piso ingresado =======*/
	carga = (pinesTeclado[primerDigito] * 10) + pinesTeclado[segundoDigito];
	/*======= Etapa de carga en el vector que almacena los pisos ingresados =======*/
	if (indice > 0)
		almacenarPisos[indice] = carga;
	else	almacenarPisos[0] = carga;
	indice++;
	}
primerDigito  = 0;
segundoDigito = 0;
indiceTeclaGuardar = 0;
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
void cancelar (void){
    
    int borrar = 0;
    
/* Ponemos un '0' en la última posicion almaceada*/
    almacenarPisos [indice-1] = borrar;
    
}
//*********************************************************************************************************************
//*********************************************************************************************************************



/*==================[declaraciones de funciones externas]====================*/

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
