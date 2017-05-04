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
#include "MEFAscensorPuertas.h"	// <= Biblioteca MEF ascensor


/*==================[definiciones y macros]==================================*/



#define LED_ASCMOVIENDO		LEDB	// Indica ascensor moviendose.
#define LED_ALARMAPABIERTA	LEDR	// Indica alarma de puerta abierta.
#define LED_PUERTAABIERTA	LEDG	// Indica puerta abierta.
#define LED_ABRIENDOPUERTA	LED1	// Indica puerta abriendose.
#define LED_CERRANDOPUERTA	LED2	// Indica puerta cerrandose.
#define LED_PBDETENIDO		LED3	// Indica Ascensor detenido o en planta baja.

// Tiempo del ciclo de parpadeo del led de alarma que undica puerta abierta, 0,5seg.
#define TALARMAPUERTA	500

// Tiempo durante el cual estara a puerta abierta, 2 segundos.
#define TPUETAABIERTA	5000

// Tiempo de espera para retornar a la Planta Baja. 5000 = 5 segundos.
#define TRETORNOPB	10000



/*==================[definiciones de datos internos]=========================*/



/*==================[definiciones de datos externos]=========================*/

/*==================[definiciones de datos globales]=========================*/
//SACAR ESTAS VARIABLES
int8_t pideConfiguracion = 0;


bool_t flagConfiguracion = 0;

// Variable que inidca el estado del ascensor (global)
estadoMEFASC_t estadoActualAsc;

// Variable que inidca el estado de la secuencia de apertura y cierre de la puerta del ascensor (global)
estadoMEFAbreCierraPuerta_t estadoActualPuerta;

// Variable que indica el piso actual en el cual se encuetra el ascensor.
int8_t pisoActual = 0;

// Variable que indica el piso destino del ascensor.
int8_t pisoDestino = 0;

// Velocidad del ascensor 1000 = 1seg. Puede cambiar por Configuracion externa
uint16_t velPisoPiso = 2000;

// Velocidad de apertura y cierre de puerta 1000 = 1s.Puede cambiar por Configuracion externa
uint16_t velAbreCierraPuerta = 2000;

// Indica el numero de pisos. Puede cambiar por Configuracion externa
uint8_t maximoDePisos =  20;

// Indica el numero de subsuelos. Puede cambiar por Configuracion externa
uint8_t MaximoDeSubsuelos = 2;

volatile uint32_t flag1DW = 0;
// Bit 0  Indica en 1 que se ejecuto el bloque de ejecucion unica del estado PARADO del ascensor.
// Bit 1  En 1 indica el pedido de ejecucion de la secuencia de apertura de puertas.
// Bit 2  En 1 indica el pedido de ejecucion de la secuencia de cierre de puertas.
// Bit 3  En 1 ya se solicito apertura de las puertas en PB.
// Bit 4  En 1 se esta pidiendo un nuevo piso, lo cual lo informa "CargaNuevoPiso".



// Bit 31



// Variable para manejo parpadeo led alarma.
delay_t timAlarPuerta;
// Variable para manejo del tiempo de apertura/cierre de la puerta.
delay_t timAbreCierraPuerta;
// Variable para manejo del tiempo que queda la puerta abierta.
delay_t timPuertaAbierta;
// Variable para manejo del tiempo que tardada el ascensor en desplazarse de un piso a otro.
delay_t timPisoPiso;
// Variable para manejo del tiempo que espera para retornar a la planta baja.
delay_t timRetornoPB;




/*==================[declaraciones de datos externos]=========================*/
extern int almacenarPisos [10];		// Vector para almacenar hasta 10 pisos ingresados correctamente.
extern int indice;				// Indice para recorrer el vector anterior.





/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/
// Prototipos de funciones
void InicializarMEFAsc(void);
void ActualizarMEFAsc(void);
void InicializarMEFPuerta(void);
void ActualizaMEFPuerta(void);







//*********************************************************************************************************************
//		Función inicializar MEF del ascensor
//*********************************************************************************************************************
void InicializarMEFAsc(void)
{
// Estado inicial.
estadoActualAsc = EN_PLANTA_BAJA;
gpioWrite (LED_PBDETENIDO, 1);


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
delayConfig(&timAbreCierraPuerta, velAbreCierraPuerta);   

// Se configura tiempo que queda la puerta abierta.
delayConfig(&timPuertaAbierta, TPUETAABIERTA);   

// Se configura tiempo que espera antes de retornar a Planta Baja.
delayConfig(&timRetornoPB, TRETORNOPB);

}
//*********************************************************************************************************************
//*********************************************************************************************************************




//*********************************************************************************************************************
//		Función que obtiene un nuevo piso
//*********************************************************************************************************************
void CargaNuevoPiso(void)
{
uint8_t i ;

// Si indice es distinto de cero significa que hay algun piso cargado en el buffer.
if (indice && !Ask_PideNuevoPisoFlag)
	{
	// Si el piso actual es distinto del piso destino, se lo fija como nuevo, si no solo se desplaza el buffer.
	if (pisoActual != almacenarPisos[0])
		{
		pisoDestino = almacenarPisos[0];
		Set_PideNuevoPisoFlag;
		}
	indice--;
	for (i=0; i<9; i++)
		almacenarPisos[i] = almacenarPisos[i+1];
	almacenarPisos[9] = 0;
	}
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
		// Se solicita la apertura de puertas, una sola vez.
		if (!Ask_PidioAperturaFlag)
			{
			Set_PidioAperturaFlag;
			Set_AbrePuertasFlag;
			}
		// Consulta si hay algun piso pendiente en el buffer, si es asi lo obtiene.
		CargaNuevoPiso();
		
		if (Ask_PideNuevoPisoFlag)
			{
			if (estadoActualPuerta == PUERTA_ABIERTA)
				Set_CierraPuertasFlag;			// Pide cerrar puertas.
			
			
			if (estadoActualPuerta == PUERTA_CERRADA)
				{
				if (pisoActual < pisoDestino)
					{
					
					// El piso destino se encuentra arriba.
					estadoActualAsc = SUBIENDO;
					Clr_PidioAperturaFlag;
					}
				else	{
					// Ante un cambio de estado se ejecuta el cierre de las puertas.
					Set_CierraPuertasFlag;
					// El piso destino se encuentra abajo.
					estadoActualAsc = BAJANDO;
					Clr_PidioAperturaFlag;
					}
				}
			}
		
		// Se consulta si pide modo Configuracion.
		if (pideConfiguracion)//COMPLETAR!!!
			{
			estadoActualAsc = MODO_CONFIGURACION;
			}

		break;
			
	case SUBIENDO:
		// Se cerraron las puertas cambia de estado?
		if (estadoActualPuerta == PUERTA_CERRADA)
			{
			gpioWrite (LED_PBDETENIDO, 0);
			gpioWrite (LED_ASCMOVIENDO, 1);
	
			// SALIDA EN EL ESTADO:
			// Se hace subir el ascensor, demorara en subir lo configurado en "velPisoPiso".
			if (delayRead(&timPisoPiso))
				{
				pisoActual = pisoActual + 1;
				//Se consulta si llego.
				if (pisoActual == pisoDestino)
					{
					// CAMBIO DE ESTADO:
					estadoActualAsc = PARADO;
					Clr_PideNuevoPisoFlag;
					gpioWrite (LED_ASCMOVIENDO, 0);
					gpioWrite (LED_PBDETENIDO, 1);
					}
				}
			}
		break;

	case BAJANDO:
		// Se cerraron las puertas cambia de estado?
		if (estadoActualPuerta == PUERTA_CERRADA)
			{
			gpioWrite (LED_PBDETENIDO, 0);
			gpioWrite (LED_ASCMOVIENDO, 1);
		
			// SALIDA EN EL ESTADO:
			// Se hace bajar el ascensor, demorara en subir lo configurado en "velPisoPiso".
			if (delayRead(&timPisoPiso))
				{
				pisoActual = pisoActual - 1;
				//Se consulta si llego.
				if (pisoActual == pisoDestino)
					{
					// CAMBIO DE ESTADO:
					estadoActualAsc = PARADO;
					Clr_PideNuevoPisoFlag;
					gpioWrite (LED_ASCMOVIENDO, 0);
					gpioWrite (LED_PBDETENIDO, 1);
					}
				}
			}
		break;

	case PARADO:
		// AL INGRESAR AL EL ESTADO SE EJECUTA POR UNICA VEZ:
		if (!Ask_AscParadoFlag)
			{
			if (estadoActualPuerta == PUERTA_CERRADA)
				Set_AbrePuertasFlag;			// MEF ascensor solicita apertura de puertas
			else if (estadoActualPuerta == PUERTA_ABIERTA)
				{
				Set_CierraPuertasFlag;			// MEF ascensor solicita cierre de puertas
				Set_AscParadoFlag;
				}
			}
			
		else if (estadoActualPuerta == PUERTA_CERRADA)
			{
			// Consulta si hay algun piso pendiente en el buffer, si es asi lo obtiene.
			CargaNuevoPiso();
			
			if (Ask_PideNuevoPisoFlag)
				{
				Clr_AscParadoFlag;
				if (pisoActual < pisoDestino) //COMPLETAR!!! no tendria que venir el mismo piso...
					{
					// El piso se encuentra arriba.
					estadoActualAsc = SUBIENDO;
					gpioWrite (LED_PBDETENIDO, 0);
					gpioWrite (LED_ASCMOVIENDO, 1);
					}
				else	{
					// El piso se encuentra abajo.
					estadoActualAsc = BAJANDO;
					gpioWrite (LED_PBDETENIDO, 0);
					gpioWrite (LED_ASCMOVIENDO, 1);
					}
				}
			
			//VER!!! si esta en plata baja!
			// Se consulta si hay que ir a PB por tiempo.
			else if (delayRead(&timRetornoPB))
				{
				Clr_AscParadoFlag;
				estadoActualAsc = YENDO_A_PLANTA_BAJA;
				pisoDestino = 0;
				gpioWrite (LED_PBDETENIDO, 0);
				gpioWrite (LED_ASCMOVIENDO, 1);
				
				Clr_PideNuevoPisoFlag;			// SACAR!!!
				}
			}
		break;

	case YENDO_A_PLANTA_BAJA:
		// Consulta si hay algun piso pendiente en el buffer, si es asi lo obtiene.
		CargaNuevoPiso();
			
		if (Ask_PideNuevoPisoFlag)	
			{
			if (pisoActual < pisoDestino)
				{
				// Ante un cambio de estado se ejecuta el cierre de las puertas.
				Set_CierraPuertasFlag;
				// El piso destino se encuentra arriba.
				estadoActualAsc = SUBIENDO;
				Clr_PidioAperturaFlag;
				}
			else	{
				// Ante un cambio de estado se ejecuta el cierre de las puertas.
				Set_CierraPuertasFlag;
				// El piso destino se encuentra abajo.
				estadoActualAsc = BAJANDO;
				Clr_PidioAperturaFlag;
				}
			}

		// SALIDA EN EL ESTADO:
		// Se hace bajar o subir el ascensor, demorara en subir lo configurado en "velPisoPiso"
		if (pisoActual > 0) //COMPLETAR!!! no tendria que venir el mismo piso...
			{
			// El piso actual se encuentra arriba.
			if (delayRead(&timPisoPiso))
				pisoActual = pisoActual - 1;
			}
		else if (pisoActual < 0) 
			{
			// El piso actual se encuentra abajo.
			if (delayRead(&timPisoPiso))
				pisoActual = pisoActual + 1;

			}
		
		// CAMBIO DE ESTADO:
		// Se consulta si llego a PB
		if (pisoActual == 0)
			{
			gpioWrite (LED_ASCMOVIENDO, 0);
			estadoActualAsc = EN_PLANTA_BAJA;
			gpioWrite (LED_PBDETENIDO, 1);
			}

		break;

	case MODO_CONFIGURACION:
		// AL INGRESAR AL EL ESTADO SE EJECUTA POR UNICA VEZ:
		// Secuencia de configuracion.
		
		
	
		// CAMBIO DE ESTADO:
		// Se completo la configuracion.
		estadoActualAsc = EN_PLANTA_BAJA;
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
		// MEF ascensor solicita apertura de puertas?
		if (Ask_AbrePuertasFlag)
			{
			Clr_AbrePuertasFlag;
			estadoActualPuerta = ABRIENDO_PUERTA;
			gpioWrite(LED_ABRIENDOPUERTA, 1);
			}
		break;

	case ABRIENDO_PUERTA:
		// CAMBIO DE ESTADO:
		// Se cumplio tiempo de apertura?
		if (delayRead(&timAbreCierraPuerta))
			{
			estadoActualPuerta = PUERTA_ABIERTA;
			gpioWrite(LED_ABRIENDOPUERTA, 0);
			gpioWrite(LED_PUERTAABIERTA, 1);
			delayRead(&timPuertaAbierta);
			}
		break;

	case PUERTA_ABIERTA:
		// CAMBIO DE ESTADO:
		// MEF ascensor solicita cierre de puertas y paso el tiempo que tiene que estar abierta la puerta?
		if (Ask_CierraPuertasFlag && delayRead(&timPuertaAbierta))
			{
			Clr_CierraPuertasFlag;
			estadoActualPuerta = INTENTANDO_CERRAR_PUERTAS;
			gpioWrite(LED_PUERTAABIERTA, 0);
			
			}
		break;

	case INTENTANDO_CERRAR_PUERTAS:
		// CAMBIO DE ESTADO:
		// Hay gente?
		if (!gpioRead(TEC1))
			estadoActualPuerta = ALARMA_PUERTA_ABIERTA;	// Hay gente.
		else	{
			estadoActualPuerta = CERRANDO_PUERTA;		// No hay gente.
			gpioWrite(LED_CERRANDOPUERTA, 1);
			}
	
		break;
	
	
	case CERRANDO_PUERTA:
		// CAMBIO DE ESTADO:
		// Se cumplio tiempo de cierre? 
		if (delayRead(&timAbreCierraPuerta))
			{
			estadoActualPuerta = PUERTA_CERRADA;
			gpioWrite(LED_CERRANDOPUERTA, 0);
			}
		// Hay gente?
		if (!gpioRead(TEC1))
			estadoActualPuerta = ABRIENDO_PUERTA;	// Hay gente.
		break;

	case ALARMA_PUERTA_ABIERTA:
		// SALIDA EN EL ESTADO:
		if (delayRead(&timAlarPuerta))
			gpioToggle(LED_ALARMAPABIERTA);		// Parpadeo led que Indica alarma de puerta abierta.
	
		// CAMBIO DE ESTADO:
		// Hay gente?
		if (gpioRead(TEC1))
			{
			estadoActualPuerta = CERRANDO_PUERTA;	// No hay gente.
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