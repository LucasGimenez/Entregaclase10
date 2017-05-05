/*============================================================================
 * Licencia: 
 * Autor:   Vegh Juan Francisco Llamazares, Lucas Andres Gimenez, Fernando Guardia, Carlos Eduardo López Aldana.
 * Fecha:   4/05/2017
 *===========================================================================*/
 
/*==================[inlcusiones]============================================*/

#include "modoConfiguracion.h"   // <= su propio archivo de cabecera
#include "sapi.h"                   // <= Biblioteca sAPI

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

bool_t flagModoConfiguracion = FALSE;

extern uint32_t velocidadEntrePisos = 0; // En segundos, de 1 en adelante
extern uint32_t velocidadPuertas = 0;    // En segundos, de 1 en adelante
extern uint32_t maximoDePisos = 0;     // De 1 a 20
extern uint32_t maximoDeSubsuelos = 1; // De 0 a 5

/*
"1 - Configurar velocidad entre piso y piso."
"2 - Configurar velocidad de apertura o cerrado de puertas."
"3 - Configurar cantidad de pisos (1 a 20)."
"4 - Configurar cantidad de subsuelos (0 a 5)."
"5 - Salir del modo configuración."
*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

void modoConfiguracion( void ){
    flagModoConfiguracion = FALSE;
    
    uint8_t receivedByte;
    uint32_t datoRecibido = 0;
    
/*================== MENU =====================*/
    
    uartWriteString( UART_USB, "Eliga una de las siguientes opciones a configurar:\r\n" );
    uartWriteString( UART_USB, "1) Configurar velocidad entre piso y piso.\r\n" );
    uartWriteString( UART_USB, "2) Configurar velocidad de apertura o cerrado de puertas.\r\n" );
    uartWriteString( UART_USB, "3) Configurar cantidad maxima de pisos (1 a 20).\r\n" );
    uartWriteString( UART_USB, "4) Configurar cantidad maxima de subsuelos (0 a 5).\r\n" );
    uartWriteString( UART_USB, "5) Salir del modo configuración.\r\n" );
    
    if( uartReadByte( UART_USB, &receivedByte ) ) {
        receivedByte = datoRecibido;  
        //AGREGAR UN CLEAR DE PANTALLA ACÁ.
    }
    
    switch (datoRecibido)
    
        case 1:
                uartWriteString( UART_USB, "Ingrese la velocidad entre piso y piso en segundos:\r\n" );
                if( uartReadByte( UART_USB, &receivedByte ) ) {
                    //AGREGAR UN CLEAR DE PANTALLA ACÁ.
                    receivedByte = velocidadEntrePisos;
                    configuracionCompletada();
                }
            break;
                
        case 2:
                uartWriteString( UART_USB, "Ingrese la velocidad de apertura o cerrado de puertas en segundos:\r\n" );
                if( uartReadByte( UART_USB, &receivedByte ) ) {
                    //AGREGAR UN CLEAR DE PANTALLA ACÁ.
                    receivedByte = velocidadPuertas;
                    configuracionCompletada();
                }
            break;
            
        case 3:
                uartWriteString( UART_USB, "Ingrese la cantidad de pisos (1 a 20):\r\n" );
                if( uartReadByte( UART_USB, &receivedByte ) ) {
                    if (receivedByte >= 1 && <= 20) {
                        //AGREGAR UN CLEAR DE PANTALLA ACÁ.
                        receivedByte = maximoDePisos;
                        configuracionCompletada();
                    }
                    
                    else {
                        //AGREGAR UN CLEAR DE PANTALLA ACÁ.
                        uartWriteString( UART_USB, "ERROR, vuelva a intentarlo !!\r\n" );
                        //AGREGAR UN CLEAR DE PANTALLA ACÁ.
                        modoConfiguración();
                    }
                }
            break;
            
            
        case 4:
                uartWriteString( UART_USB, "Ingrese la cantidad de subsuelos (0 a 5):\r\n" );
                if( uartReadByte( UART_USB, &receivedByte ) ) {
                    if (receivedByte >= 0 && <= 5) {
                        //AGREGAR UN CLEAR DE PANTALLA ACÁ.
                        receivedByte = maximoDeSubsuelos;
                        configuracionCompletada();
                    }
                    
                    else {
                        //AGREGAR UN CLEAR DE PANTALLA ACÁ.
                        uartWriteString( UART_USB, "ERROR, vuelva a intentarlo !!\r\n" );
                        //AGREGAR UN CLEAR DE PANTALLA ACÁ.
                        modoConfiguración();
                    }
                }
            break;
            
        case 5:
                configuracionCompletada();
            break;
    
}

// Funcion que devuelve TRUE si se completo la configuracion
bool_t configuracionCompletada ( void ){
   bool_t retVal = TRUE;
   
   return retVal;
}

/*==================[fin del archivo]========================================*/
