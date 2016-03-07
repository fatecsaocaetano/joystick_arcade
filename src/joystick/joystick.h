/** 
 *  @file joystick.h
 *	@details [PT-BR] Classe para fazer interface com joysticks através do DIrectInput da Microsoft. Algumas
 *	modificações foram feitas para simplificar e padronizar de acordo com o layout e comportamento dos controles
 *	das máquinas de arcade da Fatec São Caetano do Sul.
 *
 * Copyright 2001 by Jon Parise <jparise@cmu.edu>
 * All rights reserved.
 * $Id: joystick.h,v 1.1.1.1 2001/10/29 19:47:56 jon Exp $
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 * 
 *  - Neither the name of Jon Parise nor the names of contributors
 *    may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
*/
#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <windows.h>
#include <basetsd.h>
#include <dinput.h>


/**
 *  DirectInput Joystick Wrapper
*/
class Joystick
{

public:

	/**
	*	@brief Bitfield de identificação das direções de joystick. 
	*/
    enum Axis{
        UP    = 0x01 
       ,DOWN  = 0x02
       ,LEFT  = 0x04
       ,RIGHT = 0x08
    };

	/**
	 *  @details Enumeração dos 10 botões do arcade
	*/
    enum Buttons{
        BUTTON_1      /**< 0 */
       ,BUTTON_2      /**< 1 */
       ,BUTTON_3      /**< 2 */   
       ,BUTTON_4      /**< 3 */
       ,BUTTON_5      /**< 4 */
       ,BUTTON_6      /**< 5 */
       ,BUTTON_7      /**< 6 */
       ,BUTTON_8      /**< 7 */
       ,BUTTON_SELECT /**< 8 */
       ,BUTTON_START  /**< 9 */
    };


    Joystick(unsigned int id);
    ~Joystick();

    HRESULT init();
    HRESULT close();

    /**
     *  @brief Estado do botão
     *  
     *  @param [in] num Id do botão a ser verificado
     *  @return false - botão NÃO pressionado | true - botão pressionado
     *  
     *  @code
     *  //exemplo
     *  if(getButton(Joystick::BUTTON_1){
     *  	// personagem pula
     *  }
     *  @endcode
    */
    bool getButton (unsigned int num);
	
    /**
     *  @brief Eixo X do joystick da esquerda
     *  
     *  @return valor de -1 (mais a esquerda) à 1 (mais a direita)
     *  @code
     *  //exemplo
     *  float x = getAxisX();
     *  if (x < -0.05) {// anda para a esquerda}
     *  else if (x > 0.05) {// anda para a direita}
     *  @endcode
     *
    */
    float getAxisX();
	
    /**
     *  @brief Eixo Y do joystick da esquerda
     *  
     *  @return valor de -1 (mais para cima) à 1 (mais para baixo)
     *  @code
     *  //exemplo
     *  float y = getAxisY();
     *  if (Y < -0.05) {// nave vai para frente}
     *  else if (y > 0.05) {// nave vai para trás}
     *  @endcode
    */
    float getAxisY();

    /**
     *  @brief Traduz os valores dos eixos X e Y em direcionais digitais
     *  
     *  @return O retorno será a combinação do bitfiels @ref Axis conforme a direção do joystick
     *  
     *  @code
     *  //exemplo
     *  Axis dir = getAxisPosition();
     *  if(dir & Joystick::UP){
     *  	// pula
     *  }
     *  if (dir & Joystick::RIGHT){
     *  	// velocidade ++
     *  }
     *  // ...
     *  
     *  @endcode
    */
    int getAxisPosition();

    /**
     *  @brief Retorna o número de joysticks plugados
    */
    static unsigned int deviceCount();
    BOOL CALLBACK enumCallback(const DIDEVICEINSTANCE* instance, VOID* context);
    
private:
    unsigned int            id;
    unsigned int            counter;
    
    HRESULT poll(DIJOYSTATE2 *js);
    
    LPDIRECTINPUTDEVICE8    joystick;
    LPDIRECTINPUT8          interface_ptr;

};

BOOL CALLBACK enumCallback(const DIDEVICEINSTANCE* instance, VOID* context);
BOOL CALLBACK countCallback(const DIDEVICEINSTANCE* instance, VOID* counter);

#endif
