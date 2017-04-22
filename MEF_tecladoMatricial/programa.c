/*============================================================================
 * Licencia: 
 * Autor:   Vegh Juan Francisco Llamazares, Lucas Andres Gimenez, Fernando Guardia, Carlos Eduardo López Aldana.
 * Fecha:   21/04/2017
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "programa.h"   // <= su propio archivo de cabecera
#include "sapi.h"       // <= Biblioteca sAPI
#include "MEF_tecladoMatricial.h"
/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
    boardConfig();   
    
    configurarTeclado();
    
    delayConfig (&miDelay, 1000);
    
   // ---------- REPETIR POR SIEMPRE --------------------------
   while (1)
   {      

        actualizarMEF_tecladoMatricial();    
       
    }
     
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/


