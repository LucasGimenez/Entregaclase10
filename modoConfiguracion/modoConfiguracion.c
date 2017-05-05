/*============================================================================
 * Copyright 2017, Lucas Andres Gimenez, Carlos Eduardo López Aldana.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 

 * Date: 2017/04/10

 *===========================================================================*/
 
/*==================[inlcusiones]============================================*/

#include "modoConfiguracion.h"   // <= su propio archivo de cabecera
#include "sapi.h"                   // <= Biblioteca sAPI

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

bool_t flagModoConfiguracion = FALSE;

uint32_t velocidadEntrePisos = 0; // En milisegundos, de 1 en adelante
uint32_t velocidadPuertas = 0;    // En milisegundos, de 1 en adelante
uint32_t maximoDePisos = 0;     // De 1 a 20
uint32_t maximoDeSubsuelos = 1; // De 0 a 5

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
    flagModoConfiguracion = FALSE; // Buscar que hacer con este FLAG
	
	static bool_t flagCase1 = 0;
	static bool_t flagCase2 = 0;
	static bool_t flagCase3 = 0;
	static bool_t flagCase4 = 0;
	static bool_t flagCase5 = 0;
	static bool_t flagMenu  = 0;
	static bool_t flagDatoMenu = 0;
    
    uint8_t receivedByte;
    static uint32_t datoRecibido = 0;
    
/*================== MENU =====================*/
    if (!flagMenu) {
    uartWriteString( UART_USB, "Eliga una de las siguientes opciones a configurar:\r\n" );
    uartWriteString( UART_USB, "1) Configurar velocidad entre piso y piso:\r\n" );
    uartWriteString( UART_USB, "2) Configurar velocidad de apertura o cerrado de puertas:\r\n" );
    uartWriteString( UART_USB, "3) Configurar cantidad maxima de pisos (1 a 20):\r\n" );
    uartWriteString( UART_USB, "4) Configurar cantidad maxima de subsuelos (0 a 5):\r\n" );
    uartWriteString( UART_USB, "5) Salir del modo configuración.\r\n" );
	flagMenu = 1;
	}
	if (!flagDatoMenu) {
		if( uartReadByte( UART_USB, &receivedByte ) ) {
			datoRecibido = receivedByte; 
			flagDatoMenu = 1;
			//AGREGAR UN CLEAR DE PANTALLA ACÁ.
    	}
	}
	
    switch (datoRecibido)
    
        case 1:
				if (!flagCase1) {
					uartWriteString( UART_USB, "Ingrese la velocidad entre piso y piso en milisegundos:\r\n" );
					flagCase1 = 1;
				}
                if( uartReadByte( UART_USB, &receivedByte ) ) {
                    //AGREGAR UN CLEAR DE PANTALLA ACÁ.
                    velocidadEntrePisos = receivedByte;
                    flagMenu = 0;
					flagCase1 = 0;
					flagDatoMenu = 0;
					datoRecibido = 0;
                }
            break;
                
        case 2:
				if (!flagCase2) {
					uartWriteString( UART_USB, "Ingrese la velocidad de apertura o cerrado de puertas en milisegundos:\r\n" );
					flagCase2 = 1;
				}
                if( uartReadByte( UART_USB, &receivedByte ) ) {
                    //AGREGAR UN CLEAR DE PANTALLA ACÁ.
                    velocidadPuertas = receivedByte;
                    flagMenu = 0;
					flagCase1 = 0;
					flagDatoMenu = 0;
					datoRecibido = 0;
                }
            break;
            
        case 3:
				if (!flagCase3) {
					uartWriteString( UART_USB, "Ingrese la cantidad de pisos (1 a 20):\r\n" );
					flagCase3 = 1;
				}
                if( uartReadByte( UART_USB, &receivedByte ) ) {
                    if (receivedByte >= 1 && receivedByte <= 20) {
                        //AGREGAR UN CLEAR DE PANTALLA ACÁ.
                        maximoDePisos = receivedByte;
						flagMenu = 0;
						flagCase1 = 0;
						flagDatoMenu = 0;
						datoRecibido = 0;
                    }
                }
            break;
            
            
        case 4:
				if (!flagCase4) {
					uartWriteString( UART_USB, "Ingrese la cantidad de subsuelos (0 a 5):\r\n" );
					flagCase4 = 1;
				}
                if( uartReadByte( UART_USB, &receivedByte ) ) {
                    if (receivedByte >= 0 && receivedByte <= 5) {
                        //AGREGAR UN CLEAR DE PANTALLA ACÁ.
                        maximoDeSubsuelos = receivedByte;
                        flagMenu = 0;
						flagCase1 = 0;
						flagDatoMenu = 0;
						datoRecibido = 0;
                    }
                }
            break;
            
        case 5:
                //MANEJAR UN FLAG U OTRA OPCION PARA SALIR DEL MODO CONFIGURACION
            break;
	
		default:
				flagDatoMenu = 0;
			break;
}

/*==================[fin del archivo]========================================*/
