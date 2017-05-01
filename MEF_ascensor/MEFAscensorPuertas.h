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

#ifndef _MEFASCENSORPUERTAS_H_
#define _MEFASCENSORPUERTAS_H_

/*==================[inclusiones]============================================*/


/*==================[macros]=================================================*/



/*==================[typedef]================================================*/
// Nuevo tipo de datos enumerado llamado estadoMEF
typedef enum{
	EN_PLANTA_BAJA,		// 0
	SUBIENDO,		// 1
	BAJANDO,		// 2
	PARADO,			// 3
	YENDO_A_PLANTA_BAJA,	// 4
	MODO_CONFIGURACION	// 5
} estadoMEFASC_t;

typedef enum{
	PUERTA_CERRADA,			// 0
	ABRIENDO_PUERTA,		// 1
	PUERTA_ABIERTA,			// 2
	INTENTANDO_CERRAR_PUERTAS,	// 3
	CERRANDO_PUERTA,		// 4
	ALARMA_PUERTA_ABIERTA		// 5
} estadoMEFAbreCierraPuerta_t;



/*==================[external data declaration]==============================*/



/*==================[external functions declaration]=========================*/

void InicializarMEFAsc(void);
void ActualizarMEFAsc(void);
void InicializarMEFPuerta(void);
void ActualizaMEFPuerta(void);




/*==================[end of file]============================================*/
#endif /* _MEFASCENSORPUERTAS_H_ */
