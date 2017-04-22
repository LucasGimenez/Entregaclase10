/*============================================================================
 * Licencia: 
 * Autor:   Vegh Juan Francisco Llamazares, Lucas Andres Gimenez, Fernando Guardia, Carlos Eduardo López Aldana.
 * Fecha:   21/04/2017
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "MEF_tecladoMatricial.h"   // <= su propio archivo de cabecera
#include "sapi.h"                     // <= Biblioteca sAPI

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

void actualizarMEF_tecladoMatricial (void) { 
       if (ingresarDigito) {
            
            delay(500);
            
            switch (indiceTeclaGuardar){
                
                case '0':  if (pinesTeclado[segundoDigito] == 'A') {
                                guardarPisoSimple (primerDigito);
                            }
                            else if (pinesTeclado[confirmar] == 'B') {
                                cancelar();
                            }
                        break;
                            
                case '1':   if (pinesTeclado[confirmar] == 'A' ) {
                                guardarPisoDoble (primerDigito, segundoDigito);
                            }
                
                            else if (pinesTeclado[confirmar] == 'B') {
                                cancelar();
                            }
                        break;
            }
        }
}

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
                    
                case '0':  primerDigito = i * 4 + j;
                           indiceTeclaPresionada = 1;
                        break;
                
                case '1':  segundoDigito = i * 4 + j;
                            if (pinesTeclado[segundoDigito] == 'A' || 'B') {
                                indiceTeclaPresionada = 0;
                                indiceTeclaGuardar = 0;
                            }
                            else {
                                indiceTeclaPresionada = 2;
                            }
                        break;
                    
                case '2':  confirmar = i * 4 + j;
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



void guardarPisoSimple (int primerDigito){
    
/*======= Funcion que almacena el piso ingresado, de un solo digito, en el vector =======*/

    if (i > 0 ){
    pinesTeclado[primerDigito] = almacenarPisos[i];
    }
    
    else {
    pinesTeclado[primerDigito] = almacenarPisos[0];
    }
    
    i ++;
}


void guardarPisoDoble (int primerDigito, int segundoDigito){
    
/*======= Funcion que almacena el piso ingresado, de dos digitos, en el vector =======*/
    int carga = 0;

/*======= Etapa de almacenar en la variable 'carga' el piso ingresado =======*/
    if( pinesTeclado[segundoDigito] == 0 ){
        carga = pinesTeclado[primerDigito] * 10;
    }
    
        else {
            carga = primerDigito + segundoDigito + 9;
        }

/*======= Etapa de carga en el vector que almacena los pisos ingresados =======*/
    if ( i > 0 ){
        carga = almacenarPisos[i];
    }
    
        else{
            carga = almacenarPisos[0];
        }
    
    i++;
}

void cancelar (void){
    
    int borrar = 0;
    
/* Ponemos un '0' en la última posicion almaceada*/
    almacenarPisos [i-1] = borrar;
    
}
/*==================[declaraciones de funciones externas]====================*/

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
