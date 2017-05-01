/*============================================================================
 * Licencia: 
 * Autor:   Vegh Juan Francisco Llamazares, Lucas Andres Gimenez, Fernando Guardia, Carlos Eduardo López Aldana.
 * Fecha:   21/04/2017
 *===========================================================================*/

#ifndef _MEF_TECLADOMATRICIAL_H_
#define _MEF_TECLADOMATRICIAL_H_

/*==================[inclusiones]============================================*/







void cancelar (void);
void configurarTeclado(void);
void guardarPisoSimple(int primerDigito);
void guardarPisoDoble (int primerDigito, int segundoDigito);

void actualizarMEF_tecladoMatricial (void);

bool_t ingresarDigito(void);



/*==================[end of file]============================================*/
#endif /* _MEF_TECLADOMATRICIAL_H_ */
