/*============================================================================
 * Licencia: 
 * Autor:   Vegh Juan Francisco Llamazares, Lucas Andres Gimenez, Fernando Guardia, Carlos Eduardo López Aldana.
 * Fecha:   21/04/2017
 *===========================================================================*/

#ifndef _PROGRAM_H_
#define _PROGRAM_H_

/*==================[inclusiones]============================================*/

/*==================[c++]====================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[tipos de datos declarados por el usuario]===============*/

/*==================[declaraciones de datos externos]========================*/


uint16_t primerDigito  = 0; // Variable para almacenar el primer digito ingresado.
uint16_t segundoDigito = 0; // Variable para almacenar el segundo digito ingresado.
uint16_t confirmar = 0;     // Variable para almacenar si se confirma o cancela en ingreso de los datos.

uint32_t indiceTeclaPresionada = 0; // Variable que me permite saber cuantas veces fue presionado el teclado y asi saber que tecla se espera.
uint32_t indiceTeclaGuardar = 0;    // Variable que me permite conocer que digito fue ingresado para su posterior almacenamiento. (Similar a la anterior pero se usa en main).


int almacenarPisos [10];    // Vector para almacenar hasta 10 pisos ingresados correctamente.
int i = 0;                  // Indice para recorrer el vector anterior.

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
                                '1', '2', '3', 'A',
                                '4', '5', '6', 'B',
                                '7', '8', '9', 'C',
                                '*', '0', '#', 'D'
};

delay_t miDelay;


/*==================[declaraciones de funciones externas]====================*/


void cancelar (void);
void configurarTeclado(void);
void guardarPisoSimple(int primerDigito);
void guardarPisoDoble (int primerDigito, int segundoDigito);

bool_t ingresarDigito(void);

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _PROGRAM_H_ */
