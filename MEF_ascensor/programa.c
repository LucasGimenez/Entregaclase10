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

#include "programa.h"		// <= su propio archivo de cabecera
#include "sapi.h"		// <= Biblioteca sAPI
#include "driverDisplay.h"	// <= Biblioteca display 7 segmentos x 4 digitos
#include "driverTeclado.h"	// <= Biblioteca teclado 4x4
#include "mefAscensor.h"	// <= Biblioteca MEF ascensor

/*==================[definiciones y macros]==================================*/
#define setbit32(var, bit)               ((var) |=  (uint32_t)((uint32_t)1<<(uint32_t)(bit)))
#define clrbit32(var, bit)             ((var) &= ~(uint32_t)((uint32_t)1<<(uint32_t)(bit)))
#define querybit32(var, bit)             ((bool_t)((var)>>(uint32_t)(bit)) & ((uint32_t)1))

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
	PUERTA_CERRADA,		// 0
	ABRIENDO_PUERTA,	// 1
	PUERTA_ABIERTA,		// 2
	CERRANDO_PUERTA,	// 3
	ALARMA_PUERTA_ABIERTA	// 4
} estadoMEFAbreCierraPuerta_t;


#define LED_ASCMOVIENDO		LEDB	// Indica ascensor moviendose.
#define LED_ALARMAPABIERTA	LEDR	// Indica alarma de puerta abierta.
#define LED_PUERTAABIERTA	LEDG	// Indica puerta abierta.
#define LED_ABRIENDOPUERTA	LED1	// Indica puerta abriendose.
#define LED_CERRANDOPUERTA	LED2	// Indica puerta cerrandose.
#define LED_PBDETENIDO		LED3	// Indica Ascensor detenido o en planta baja.

// Tiempo del ciclo de parpadeo del led de alarma que undica puerta abierta, 0,5seg.
#define TALARMAPUERTA	500

// Tiempo durante el cual estara a puerta abierta, 2 segundos.
#define TPUETAABIERTA	2000

// Tiempo de espera para retornar a la Planta Baja. 5000 = 5 segundos.
#define TRETORNOPB	5000




/*==================[definiciones de datos internos]=========================*/



/*==================[definiciones de datos externos]=========================*/

/*==================[definiciones de datos globales]=========================*/

// Variable que inidca el estado del ascensor (global)
estadoMEFASC_t estadoActualAsc;

// Variable que inidca el estado de la secuencia de apertura y cierre de la puerta del ascensor (global)
estadoMEFAbreCierraPuerta_t estadoActualPuerta;

// Variable que indica el piso actual en el cual se encuetra el ascensor.
int8_t pisoActual = 0;

// Variable que indica el piso destino del ascensor.
int8_t pisoDestino = 0;

// Velocidad del ascensor 1000 = 1seg. Puede cambiar por Configuracion externa
uint16_t velPisoPiso = 1000;

// Velocidad de apertura y cierre de puerta 1000 = 1s.Puede cambiar por Configuracion externa
uint16_t velAbreCierraPuerta = 1000;

// Indica el numero de pisos. Puede cambiar por Configuracion externa
uint8_t maximoDePisos =  20;

// Indica el numero de subsuelos. Puede cambiar por Configuracion externa
uint8_t MaximoDeSubsuelos = 2;

volatile uint32_t flag1DW = 0;
// Bit 0  Indica en 1 que se ehecuto el bloque de ejecucion unica del estado PARADO del ascensor.

// Bit 31
#define Set_AscParadoFlag	setbit32(flag1DW, 0)
#define Clr_AscParadoFlag	clrbit32(flag1DW, 0)
#define Ask_AscParadoFlag	querybit32(flag1DW, 0)


// Variable para manejo parpadeo led alarma.
delay_t timAlarPuerta;
// Variable para manejo del tiempo de apertura de la puerta.
delay_t timAbrePuerta;
// Variable para manejo del tiempo de cierre de la puerta.
delay_t timCierraPuerta;
// Variable para manejo del tiempo que tardada el ascensor en desplazarse de un piso a otro.
delay_t timPisoPiso;
// Variable para manejo del tiempo que espera para retornar a la planta baja.
delay_t timRetornoPB;


/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/
// Prototipos de funciones
void InicializarMEFAsc(void);
void ActualizarMEFAsc(void);
void InicializarMEFPuerta(void);
void ActualizaMEFPuerta(void);









/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

// ---------- CONFIGURACIONES ------------------------------
// Inicializar y configurar la plataforma
boardConfig();   


display7SegmentosConfigurarPines(); // Configuración de pines para el display 7 segmentos


configurarTecladoMatricial(); // Configurar teclado matricial


// Se inicializa la MEF que maneja el ascensor.
InicializarMEFAsc();

// Se inicializa la MEF que maneja la puerta del ascensor.
InicializarMEFPuerta();





// ---------- REPETIR POR SIEMPRE --------------------------
while(TRUE)
{      




// Función Actualizar MEF del Ascensor.
ActualizarMEFAsc();






} 


return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/


//*********************************************************************************************************************
//		Función inicializar MEF del ascensor
//*********************************************************************************************************************
void InicializarMEFAsc(void)
{
// Estado inicial.
estadoActualAsc = EN_PLANTA_BAJA;


// Se Configura el tiempo de desplazamiento del ascensor.
delayConfig(&timPisoPiso, velPisoPiso);   

// Se Configura el tiempo de espera para retornar a la PB.
//delayConfig(&timRetornoPB, TRETORNOPB);   

// Resetea flag que indica ejecucion unica de bloque dentro de estado PARADO.
Clr_AscParadoFlag;

// Ver las otras variables!!!


}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//		Función inicializar MEF de la puerta del ascensor
//*********************************************************************************************************************
void InicializarMEFPuerta(void)
{
// Estado inicial.
estadoActualPuerta = PUERTA_CERRADA;

// Se Configura el tiempo para el parpadeo del led de alarma indicador de puerta abierta, 0,5seg.
delayConfig(&timAlarPuerta, TALARMAPUERTA);   

// Se configura tiempo de apertura de la puerta.
delayConfig(&timAbrePuerta, velAbreCierraPuerta);   

// Se configura tiempo de cierre de la puerta.
delayConfig(&timCierraPuerta, velAbreCierraPuerta);   

}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//		Función Actualizar MEF del Ascensor
//*********************************************************************************************************************
void ActualizarMEFAsc(void)
{
switch(estadoActualAsc)
	{
	case EN_PLANTA_BAJA:
		// AL INGRESAR AL EL ESTADO SE EJECUTA POR UNICA VEZ:
		// Apertura de puertas.

		gpioWrite (LED_PBDETENIDO, 1);

		
		// CAMBIO DE ESTADO:
		// Se consulta si hay nuevo piso, si hay se pasa al estado subiendo o bajando.
		// VER!!! es mismo piso.
		if (0)//COMPLETAR!!!
			{
			
			if (pisoActual < pisoDestino) //COMPLETAR!!! no tendria que venir el mismo piso...
				{
				// Ante un cambio de estado se ejecuta el cierre de las puertas.
				// Si se cerraron las puertas cambia de estado.
				
				// El piso destino se encuentra arriba.
				estadoActualAsc = SUBIENDO;
				gpioWrite (LED_PBDETENIDO, 0);
				gpioWrite (LED_ASCMOVIENDO, 1);
				}
			else	{
				// Ante un cambio de estado se ejecuta el cierre de las puertas.
				// Si se cerraron las puertas cambia de estado.
				
				// El piso destino se encuentra abajo.
				estadoActualAsc = BAJANDO;
				gpioWrite (LED_PBDETENIDO, 0);
				gpioWrite (LED_ASCMOVIENDO, 1);
				}
			}
		
		// Se consulta si pide modo Configuracion.
		if (0)//COMPLETAR!!!
			{
			
			// Ante un cambio de estado se ejecuta el cierre de las puertas.
			
			// Si se cerraron las puertas cambia de estado.
			estadoActualAsc = MODO_CONFIGURACION;
		
			}

		break;
			
	case SUBIENDO:
		// SALIDA EN EL ESTADO:
		// Se hace subir el ascensor, demorara en subir lo configurado en "velPisoPiso".
		if (delayRead(&timPisoPiso))
			{
			pisoActual = pisoActual + 1;		// VER RANGOS!!!
			//Se consulta si llego.
			if (pisoActual == pisoDestino)		//COMPLETAR!!!
				{
				// CAMBIO DE ESTADO:
				estadoActualAsc = PARADO;
				gpioWrite (LED_ASCMOVIENDO, 0);
				gpioWrite (LED_PBDETENIDO, 1);
				}
			}
		break;

	case BAJANDO:
		
		// SALIDA EN EL ESTADO:
		// Se hace bajar el ascensor, demorara en subir lo configurado en "velPisoPiso".
		if (delayRead(&timPisoPiso))
			{
			pisoActual = pisoActual - 1;		// VER RANGOS!!!
			//Se consulta si llego.
			if (pisoActual == pisoDestino)		//COMPLETAR!!!
				{
				// CAMBIO DE ESTADO:
				estadoActualAsc = PARADO;
				gpioWrite (LED_ASCMOVIENDO, 0);
				gpioWrite (LED_PBDETENIDO, 1);
				}
			}		
		break;

	case PARADO:
		// AL INGRESAR AL EL ESTADO SE EJECUTA POR UNICA VEZ:
		if (!Ask_AscParadoFlag)
			{
			Set_AscParadoFlag;
			// Se pasa a MEF de las puertas que ejecuta una secuencia completa de apertura y cierre de puertas con las condiciones intermedias.
			
			
			// Se inicializa temporizador, el cual permitira que al cabo de x tiempo el ascensor regrese automaticamente a la planta baja.
			delayConfig(&timRetornoPB, TRETORNOPB);   // ANALIZAR!!!
			}
		
		
				
		// CAMBIO DE ESTADO:
		// Se consulta si hay nuevo piso, si hay se pasa al estado subiendo o bajando.
		if (0)//COMPLETAR!!!
			{
			Clr_AscParadoFlag;
			if (pisoActual < pisoDestino) //COMPLETAR!!! no tendria que venir el mismo piso...
				{
				// Ante un cambio de estado se ejecuta el cierre de las puertas.
				// Si se cerraron las puertas cambia de estado.
				
				// El piso se encuentra arriba.
				estadoActualAsc = SUBIENDO;
				gpioWrite (LED_PBDETENIDO, 0);
				gpioWrite (LED_ASCMOVIENDO, 1);
				}
			else	{
				// Ante un cambio de estado se ejecuta el cierre de las puertas.
				// Si se cerraron las puertas cambia de estado.
				
				// El piso se encuentra abajo.
				estadoActualAsc = BAJANDO;
				gpioWrite (LED_PBDETENIDO, 0);
				gpioWrite (LED_ASCMOVIENDO, 1);
				}
			}
		
		//VER!!! si esta en plata baja!
		// Se consulta si hay que ir a PB por tiempo.
		if (delayRead(&timRetornoPB))
			{
			Clr_AscParadoFlag;
			estadoActualAsc = YENDO_A_PLANTA_BAJA;
			pisoDestino = 0;
			gpioWrite (LED_PBDETENIDO, 0);
			gpioWrite (LED_ASCMOVIENDO, 1);
			}
	
		break;

	case YENDO_A_PLANTA_BAJA:
		// SALIDA EN EL ESTADO:
		// Se hace bajar o subir el ascensor, demorara en subir lo configurado en "velPisoPiso"
		
		if (pisoActual > pisoDestino) //COMPLETAR!!! no tendria que venir el mismo piso...
				{
				// El piso actual se encuentra arriba.
				if (delayRead(&timPisoPiso))
					pisoActual = pisoActual - 1;		// VER RANGOS!!!
				}
		else if (pisoActual < pisoDestino) //COMPLETAR!!! no tendria que venir el mismo piso...
				{
				// El piso actual se encuentra abajo.
				if (delayRead(&timPisoPiso))
					pisoActual = pisoActual + 1;		// VER RANGOS!!!

				}
		
	
		// CAMBIO DE ESTADO:
		// Se consulta si hay nuevo piso, si hay se pasa al estado subiendo o bajando.
		// VER!!! es mismo piso.
		if (0)//COMPLETAR!!!
			{
			
			if (pisoActual < pisoDestino) //COMPLETAR!!! no tendria que venir el mismo piso...
				{
				// Ante un cambio de estado se ejecuta el cierre de las puertas.
				// Si se cerraron las puertas cambia de estado.
				
				// El piso destino se encuentra arriba.
				estadoActualAsc = SUBIENDO;
				}
			else	{
				// Ante un cambio de estado se ejecuta el cierre de las puertas.
				// Si se cerraron las puertas cambia de estado.
				
				// El piso destino se encuentra abajo.
				estadoActualAsc = BAJANDO;
				}
			}

		// Se consulta si llego a PB
		if (pisoActual == pisoDestino) //COMPLETAR!!! no tendria que venir el mismo piso...
			{
			gpioWrite (LED_ASCMOVIENDO, 0);
			estadoActualAsc = EN_PLANTA_BAJA;
			}

		break;

	case MODO_CONFIGURACION:
		// AL INGRESAR AL EL ESTADO SE EJECUTA POR UNICA VEZ:
		// Secuencia de configuracion.
		
		
	
		// CAMBIO DE ESTADO:
		// Se completo la configuracion.

		break;

		
	
	

	default:
		// Si "estadoActualAsc" adquiere un valor invalido se configura un estado de recuperacion.
		InicializarMEFAsc();
		break;
 	}      
}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//		Función Actualizar MEF de la puerta del ascensor
//*********************************************************************************************************************
void ActualizaMEFPuerta(void)
{
switch(estadoActualPuerta)
	{
	case PUERTA_CERRADA:
		// CAMBIO DE ESTADO:
		// MEF ascensor solicita apertura de puertas.
		if (0)//COMPLETAR!!!
			{
			estadoActualAsc = ABRIENDO_PUERTA;
			gpioWrite(LED_ABRIENDOPUERTA, 1);
			}
		break;

	case ABRIENDO_PUERTA:
		// CAMBIO DE ESTADO:
		// Se cumplio tiempo de apertura?
		if (delayRead(&timAbrePuerta))
			{
			estadoActualAsc = PUERTA_ABIERTA;
			gpioWrite(LED_ABRIENDOPUERTA, 0);
			gpioWrite(LED_PUERTAABIERTA, 1);
			}
		break;

	case PUERTA_ABIERTA:
		// CAMBIO DE ESTADO:
		// MEF ascensor solicita apertura de puertas.
		if (0)//COMPLETAR!!!
			{
			estadoActualAsc = CERRANDO_PUERTA;
			gpioWrite(LED_PUERTAABIERTA, 0);
			gpioWrite(LED_CERRANDOPUERTA, 1);
			}
		break;

	case CERRANDO_PUERTA:
		// CAMBIO DE ESTADO:
		// Se cumplio tiempo de cierre? 
		if (delayRead(&timCierraPuerta))
			{
			gpioWrite(LED_CERRANDOPUERTA, 0);
			// Hay gente?
			if (!gpioRead(TEC1))
				estadoActualAsc = ALARMA_PUERTA_ABIERTA;	// Hay gente.
			else estadoActualAsc = PUERTA_CERRADA;			// No hay gente.
			}
		break;

	case ALARMA_PUERTA_ABIERTA:
		// SALIDA EN EL ESTADO:
		if (delayRead(&timAlarPuerta))
			gpioToggle(LED_ALARMAPABIERTA);		// Parpadeo led que Indica alarma de puerta abierta.
	
		// CAMBIO DE ESTADO:
		// Hay gente?
		if (gpioRead(TEC1))
			{
			estadoActualAsc = PUERTA_CERRADA;	// No hay gente.
			gpioWrite(LED_ALARMAPABIERTA, 0);
			}
		break;


	default:
		// Si "estadoActualPuerta" adquiere un valor invalido se configura un estado de recuperacion.
		InicializarMEFPuerta();
		break;
 	}      
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