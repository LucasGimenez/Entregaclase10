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

#include "sapi.h"		// <= Biblioteca sAPI
#include "Ascensor.h"		// <= su propio archivo de cabecera
#include "driverDisplay.h"	// <= Biblioteca display 7 segmentos x 4 digitos
#include "MEF_tecladoMatricial.h"	// <= Biblioteca teclado 4x4
#include "MEFAscensorPuertas.h"	// <= Biblioteca MEF ascensor

/*==================[definiciones y macros]==================================*/




/*==================[definiciones de datos internos]=========================*/



/*==================[definiciones de datos globales]=========================*/
//SACAR ESTAS VARIABLES
delay_t timSerial;




/*==================[declaraciones de datos externos]=========================*/
extern int8_t pideNuevoPiso;
extern int8_t pisoDestino;
extern int almacenarPisos[10];
extern int indice;
extern estadoMEFASC_t estadoActualAsc;

/*==================[declaraciones de funciones externas]====================*/
extern void EnviaEstadoInterno(void);
extern void ConfigDisplay(void);
extern void ActualizarDisplay(void);
extern void InicializarMEF_tecladoMatrical(void);
extern void ActualizarMEF_tecladoMatricial (void);
extern void InicializarMEFAsc(void);
extern void InicializarMEFPuerta(void);
extern void ActualizarMEFAsc(void);
extern void ActualizaMEFPuerta(void);



/*==================[declaraciones de funciones internas]====================*/
bool_t IntTimer (void *ptr);







//*************************************************************************************************
//			Interrupción cada 5ms
//*************************************************************************************************
bool_t IntTimer (void *ptr)
{


// Llama a la rutina que actualiza el display de 4 digitos de 7 segmentos.
ActualizarDisplay();
	

return 1;
}
//*************************************************************************************************
//*************************************************************************************************



/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

// ---------- CONFIGURACIONES ------------------------------
// Inicializar y configurar la plataforma
boardConfig();   

// Interupcion cada 5ms.
tickConfig (5, IntTimer);

// Configuración de pines para el display 7 segmentos
ConfigDisplay();

// Se inicializa la MEF que maneja el teclado matricial
InicializarMEF_tecladoMatrical();


// Se inicializa la MEF que maneja el ascensor.
InicializarMEFAsc();

// Se inicializa la MEF que maneja la puerta del ascensor.
InicializarMEFPuerta();



// UART_USB a 115200 baudios.
uartConfig( UART_USB, 115200 );
delayConfig(&timSerial, 200);   




// ---------- REPETIR POR SIEMPRE --------------------------
while(TRUE)
{      

// Función Actualizar MEF del Teclado.
ActualizarMEF_tecladoMatricial();


// Función Actualizar MEF del Ascensor.
ActualizarMEFAsc();


// Función Actualizar MEF de las Puertas.
ActualizaMEFPuerta();








// Codigo de prueba, presionando la tecla 4 se cargan valores en las 10 posiciones del buffer de pisos
// y de fija indice a 10.
if (delayRead(&timSerial))
	{
	if (estadoActualAsc != MODO_CONFIGURACION)
		EnviaEstadoInterno();
	
	if (!gpioRead(TEC4))
		{
		almacenarPisos[0] = 6;
		almacenarPisos[1] = 9;
		almacenarPisos[2] = 12;
		almacenarPisos[3] = 17;
		almacenarPisos[4] = 20;
		almacenarPisos[5] = 10;
		almacenarPisos[6] = -5;
		almacenarPisos[7] = 5;
		almacenarPisos[8] = -3;
		almacenarPisos[9] = 8;
		indice = 10;
		}
	}
} 


return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/








//*********************************************************************************************************************
//
//*********************************************************************************************************************
// AL INGRESAR AL EL ESTADO SE EJECUTA POR UNICA VEZ:
// SALIDA EN EL ESTADO:
// CAMBIO DE ESTADO:


//*********************************************************************************************************************
//*********************************************************************************************************************