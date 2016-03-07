/**
 *  @file types.h
 *  @brief Arquivo cont�ndo as defini��es comuns aos demais m�dulos
 *  do projeto
 */

#ifndef __TYPES_H__
#define __TYPES_H__

#include <math.h>
#include <iostream>
/**
* Macro para facilitar as contas utilizando o @ref NUM_OBJECTS_DEFINE.
* Exemplo: if(player.collision_mask & MASK_BIT(MST)){...}
*/
#define MASK_BIT(x) (long int)( 1 << x)

#ifndef M_PI
	#define M_PI		3.14159265358979323846
#endif

/**
 *  Vetor2D b�sico
 */
typedef struct vetor2d_type{
	float x; /**< Decomposi��o x do vetor*/
	float y; /**< Decomposi��o y do vetor*/
	
	/**
	 *  @brief Calcula o m�dulo de vetor
	 *  @return O valor do m�dulo
	 */
	float modulo(){
		return sqrt(((x*x) + (y*y)));
	}
	
	/**
	 *  @brief Inicializa o vetor utilizando m�dulo e angulo em graus
	 *  
	 *  @param [in] modulo M�dulo do vetor
	 *  @param [in] angle  Angulo em graus do vetor em rela��o ao eixo x 
	 *  @return Return_Description
	 */
	void setVector (float modulo, float angle){
		angle = (angle*M_PI)/180;
		x = cos(angle)*modulo;
		y = sin(angle)*modulo;
	}
	
	/**
	*  Soma ao vetor um outro vetor
	*/
	void sum(vetor2d_type *vec){
		x+= vec->x;
		y+= vec->y;
	};
}vetor2d_type;

#endif
