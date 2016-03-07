#ifndef _GRAFICOS_H__
#define _GRAFICOS_H__

#include "../h/types.h"
#include "../bgi/graphics.h"
//definições das  propriedades de tela
#define SCREEN_W 1280
#define SCREEN_H 600
#define SCREEN_CENTER SCREEN_W/2
#define PLAYER_SPOT SCREEN_W/3
#define BGCOLOR COLOR(0,0,0)


// Tipos ----------------------------------------------

/**
* Estrutura de manipulação gráfica dos objetos
*/
typedef struct{
	void *img; 		/**< Imagem que será impressa*/
	void *msk; 		/**< Imagem que será usada como mascara*/
	bool masked;	/**< Ela pode ter ou não uma máscara*/
	float h;	 	/**< Altura da imagem*/
	float w; 		/**< Largura da imagem*/
}graph_data_type;
	
// Protótipo das funções ------------------------------
void updateScreen();
void print(vetor2d_type pos, graph_data_type *obj);
void erase();
void graphInitObjects(graph_data_type *objeto, const char* caminho, const char* caminhomsk="");
void printTxt(char *texto, vetor2d_type pos);
void fontSize(int char_size=1);

#endif
