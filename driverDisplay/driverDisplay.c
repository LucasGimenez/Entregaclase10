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
#include "driverDisplay.h"	// <= Biblioteca display 7 segmentos x 4 digitos

/*==================[definiciones y macros]==================================*/

// Define los pines de los segmentos del display.
#define segmento_a	GPIO0		// a
#define segmento_b	GPIO1           // b
#define segmento_c	GPIO2           // c
#define segmento_d	GPIO3           // d
#define segmento_e	GPIO4           // e
#define segmento_f	GPIO5           // f
#define segmento_g	GPIO6           // g
#define segmento_dp	GPIO7           // dp
// Define los pines que controlan cada dígito.
#define digito_u	LCD1
#define digito_d	LCD2
#define digito_c	LCD3
#define digito_um	LCD4



/*==================[definiciones de datos internos]=========================*/
uint8_t segmentosdisplayB[8] = {segmento_a, segmento_b, segmento_c, segmento_d, segmento_e, segmento_f, segmento_g, segmento_dp};

uint8_t quedisplayB[4] = {digito_u, digito_d, digito_c, digito_um};

uint8_t numeroadisplayB[10] = {
	0b11000000,	// 0
	0b11111001,	// 1
	0b10100100,	// 2
	0b10110000,	// 3
	0b10011001,	// 4
	0b10010010,	// 5
	0b10000010,	// 6
	0b11111000,	// 7
	0b10000000,	// 8
	0b10010000	// 9
};

uint8_t secuenciaB = 0;


uint8_t unidadesB = 0;
uint8_t decenasB = 0;
uint8_t centenasB = 0;
uint8_t unidadesmilB = 0;



/*==================[declaraciones de datos externos]=========================*/
extern int8_t pisoActual;






/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/


//*************************************************************************************************
//			Configura pines para los dígitos
//*************************************************************************************************
void ConfigDisplay(void)
{
uint8_t i = 0;

// Se setean los pines correspondientes a los segmentos como salida y en cero.
for (i=0; i<8; i++)
	{
	gpioConfig (segmentosdisplayB[i], GPIO_OUTPUT);
	gpioWrite (segmentosdisplayB[i], 1);
	}

// Se setean los pines correspondientes al control de los anodos común de los dígitos, en salida y cero.
for (i=0; i<4; i++)
	{
	gpioConfig (quedisplayB[i], GPIO_OUTPUT);
	gpioWrite (quedisplayB[i], 1);
	}
	
}
//*************************************************************************************************
//*************************************************************************************************





//*********************************************************************************************************************
//			Actualiza un dígito
//*********************************************************************************************************************
void SacaDigito(uint8_t numero, uint8_t lcdx)
{
uint8_t i = 0;

gpioWrite (LCD1, 1);
gpioWrite (LCD2, 1);
gpioWrite (LCD3, 1);
gpioWrite (LCD4, 1);
gpioWrite (lcdx, 0);

for (i=0; i<=7; i++)
	gpioWrite (segmentosdisplayB[i], (numeroadisplayB[numero] & (1 << i)));
}
//*********************************************************************************************************************
//*********************************************************************************************************************



//*********************************************************************************************************************
//
//*********************************************************************************************************************
void ConvierteByte(uint8_t X, uint8_t *p)
{   
if (X)
	{

	p[2] = X/100;
	X = X%100;
	
	p[1] = X/10;
	X = X%10;
	
	p[0] = X;
	}
else	{
	p[2] = 0;
	p[1] = 0;
	p[0] = 0;
	}
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//*********************************************************************************************************************
void PonPisoActualDisplay(void)
{

uint8_t valores[3];


ConvierteByte(pisoActual, valores);
unidadesmilB = 0;
centenasB = valores[2];
decenasB = valores[1];
unidadesB = valores[0];



switch (secuenciaB){
	case 0:
		SacaDigito (unidadesB, LCD1);
		secuenciaB++;
		break;
	case 1:
		SacaDigito (decenasB, LCD2);
		secuenciaB++;
		break;
	case 2:
		SacaDigito (centenasB, LCD3);
		secuenciaB++;
		break;
	case 3:
		SacaDigito (unidadesmilB, LCD4);
	
	default:
		secuenciaB = 0;
		break;
	}
	
}
//*********************************************************************************************************************
//*********************************************************************************************************************






/*==================[fin del archivo]========================================*/
