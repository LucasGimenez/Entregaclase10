/*============================================================================
 * Copyright 2017, Vegh Juan Francisco Llamazares, Lucas Andres Gimenez, Fernando Guardia, Carlos Eduardo López Aldana.
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
#define setbit32(var, bit)               ((var) |=  (uint32_t)((uint32_t)1<<(uint32_t)(bit)))
#define clrbit32(var, bit)             ((var) &= ~(uint32_t)((uint32_t)1<<(uint32_t)(bit)))
#define querybit32(var, bit)             ((bool_t)((var)>>(uint32_t)(bit)) & ((uint32_t)1))



/*==================[definiciones de datos internos]=========================*/


/*==================[definiciones de datos globales]=========================*/
//SACAR ESTAS VARIABLES
delay_t timSerial;


/*==================[declaraciones de funciones internas]====================*/







/*==================[declaraciones de datos externos]=========================*/

extern int8_t pideNuevoPiso;
extern int8_t pisoDestino;

/*==================[declaraciones de funciones externas]====================*/
extern void EstadoInterno(void);
// extern void configurarTeclado(void);
// extern void actualizarMEF_tecladoMatricial (void);
extern void ConfigDisplay(void);
extern void PonPisoActualDisplay(void);




//*************************************************************************************************
//			Interrupción cada 5ms
//*************************************************************************************************
bool_t IntTimer (void *ptr)
{

PonPisoActualDisplay();
	

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

ConfigDisplay(); // Configuración de pines para el display 7 segmentos


configurarTeclado(); // Configurar teclado matricial


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
actualizarMEF_tecladoMatricial();

// Función Actualizar MEF del Ascensor.
ActualizarMEFAsc();


// Función Actualizar MEF de las Puertas.
ActualizaMEFPuerta();


if (delayRead(&timSerial))
	{
	EstadoInterno();
	
	if (!gpioRead(TEC4))
		{
//		pideNuevoPiso = 1;
//		pisoDestino = 5;
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