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
#include "Test.h"		// <= su propio archivo de cabecera
#include "MEFAscensorPuertas.h"	// <= Biblioteca MEF ascensor

/*==================[definiciones y macros]==================================*/
#define setbit32(var, bit)               ((var) |=  (uint32_t)((uint32_t)1<<(uint32_t)(bit)))
#define clrbit32(var, bit)             ((var) &= ~(uint32_t)((uint32_t)1<<(uint32_t)(bit)))
#define querybit32(var, bit)             ((bool_t)((var)>>(uint32_t)(bit)) & ((uint32_t)1))


/*==================[definiciones de datos internos]=========================*/


/*==================[definiciones de datos globales]=========================*/
//SACAR ESTAS VARIABLES
uint8_t numeroEnString[10];
uint32_t pasos = 0;


/*==================[declaraciones de funciones internas]====================*/
// Prototipos de funciones
// void EstadoInterno(void);
char* itoa(int value, char* result, int base);






/*==================[declaraciones de datos externos]=========================*/
extern int8_t pideNuevoPiso;
extern int8_t pisoActual;
extern int8_t pisoDestino;
extern estadoMEFASC_t estadoActualAsc;
extern estadoMEFAbreCierraPuerta_t estadoActualPuerta;

extern int almacenarPisos [10];		// Vector para almacenar hasta 10 pisos ingresados correctamente.
extern int indice;				// Indice para recorrer el vector anterior.
extern uint16_t primerDigito; // Variable para almacenar el primer digito ingresado.
extern uint16_t segundoDigito; // Variable para almacenar el segundo digito ingresado.
extern uint32_t indiceTeclaPresionada;



//*********************************************************************************************************************
//
//*********************************************************************************************************************
/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.

 */
char* itoa(int value, char* result, int base)
{
   // check that the base if valid
   if (base < 2 || base > 36) { *result = '\0'; return result; }

   char* ptr = result, *ptr1 = result, tmp_char;
   int tmp_value;

   do {
      tmp_value = value;
      value /= base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
   } while ( value );

   // Apply negative sign
   if (tmp_value < 0) *ptr++ = '-';
   *ptr-- = '\0';
   while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
   }
   return result;
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
void uartWriteMiDato(char* str, int value, int base)
{

uartWriteString(UART_USB, str);
itoa( value, numeroEnString, base);         
uartWriteString( UART_USB, numeroEnString );  
uartWriteString( UART_USB, "\r\n" );  

}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//
//*********************************************************************************************************************
void EstadoInterno(void)
{


uartWriteMiDato("almacenarPisos[0] = ", almacenarPisos[0], 10);
uartWriteMiDato("almacenarPisos[1] = ", almacenarPisos[1], 10);
uartWriteMiDato("almacenarPisos[2] = ", almacenarPisos[2], 10);
uartWriteMiDato("almacenarPisos[3] = ", almacenarPisos[3], 10);
uartWriteMiDato("almacenarPisos[4] = ", almacenarPisos[4], 10);
uartWriteMiDato("almacenarPisos[5] = ", almacenarPisos[5], 10);
uartWriteMiDato("almacenarPisos[6] = ", almacenarPisos[6], 10);
uartWriteMiDato("almacenarPisos[7] = ", almacenarPisos[7], 10);
uartWriteMiDato("almacenarPisos[8] = ", almacenarPisos[8], 10);
uartWriteMiDato("almacenarPisos[9] = ", almacenarPisos[9], 10);


uartWriteMiDato("Bandera Pasos = ", pasos, 10);

uartWriteMiDato("IndiceTeclaPresionada = ", indiceTeclaPresionada, 10);

uartWriteMiDato("PrimerDigito = ", primerDigito, 10);

uartWriteMiDato("SegundoDigito = ", segundoDigito, 10);

uartWriteMiDato("Indice = ", indice, 10);



uartWriteMiDato ("Piso Destino = ", pisoDestino, 10);

uartWriteMiDato ("Piso Actual = ", pisoActual, 10);


	

uartWriteString(UART_USB, "estadoActualAsc = ");
switch(estadoActualAsc)
	{
	case EN_PLANTA_BAJA:
		uartWriteString(UART_USB, "En Planta Baja\r\n");
	
		break;
			
	case SUBIENDO:
		uartWriteString(UART_USB, "Subiendo\r\n");
	
		break;

	case BAJANDO:
		uartWriteString(UART_USB, "Bajando\r\n");	
	
		break;

	case PARADO:
		uartWriteString(UART_USB, "Parado\r\n");	
	
		break;

	case YENDO_A_PLANTA_BAJA:
		uartWriteString(UART_USB, "Yendo a Planta Baja\r\n");	
	
		break;

	case MODO_CONFIGURACION:
		uartWriteString(UART_USB, "Modo Configuracion\r\n");
		
		break;

	default:

		break;
 	}      	
	
	
uartWriteString(UART_USB, "estadoActualPuerta = ");	
switch(estadoActualPuerta)
	{
	case PUERTA_CERRADA:
		uartWriteString(UART_USB, "Puertas Cerradas\r\n");

		break;

	case ABRIENDO_PUERTA:
		uartWriteString(UART_USB, "Abriendo Puertas\r\n");
		
		break;

	case PUERTA_ABIERTA:
		uartWriteString(UART_USB, "Puertas Abiertas\r\n");
		
		break;

	case INTENTANDO_CERRAR_PUERTAS:
		uartWriteString(UART_USB, "Intentado Cerrar Puertas\r\n");
		
		break;
	
	
	case CERRANDO_PUERTA:
		uartWriteString(UART_USB, "Cerrando Puertas\r\n");
		
		break;

	case ALARMA_PUERTA_ABIERTA:
		uartWriteString(UART_USB, "Alarma Puertas Abiertas\r\n");
		
		break;


	default:

		break;
 	}      

uartWriteString(UART_USB, "\r\n");

	
	
}
//*********************************************************************************************************************
//*********************************************************************************************************************








//*********************************************************************************************************************
//
//*********************************************************************************************************************
// AL INGRESAR AL EL ESTADO SE EJECUTA POR UNICA VEZ:
// SALIDA EN EL ESTADO:
// CAMBIO DE ESTADO:


//*********************************************************************************************************************
//*********************************************************************************************************************