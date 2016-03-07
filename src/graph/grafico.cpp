#include "grafico.h"
#include "../bgi/graphics.h"
#include<iostream>


#define TO_GROUND_LEVEL(y) ((SCREEN_H-10)- y)



using namespace std;


/**
 *  @brief Imprime na tela imagem contida no graph_dta_type passado, se tiver máscara faz o uso dela para simular transparência
 *  
 *  @param [in] pos  Posição do objeto (só será impresso se estiver dentro dos limites da tela @see SCREEN_H @see SCREEN_W
 *  @param [in] obj  Objeto a ser impresso
 */
void print(vetor2d_type pos, graph_data_type *obj){
	int mode = COPY_PUT;
	
	if(obj->msk){
	 	putimage(pos.x, pos.y, obj->msk, AND_PUT);
	 	mode = OR_PUT;
	 }
	
	putimage(pos.x, pos.y, obj->img, mode);	
}

/**
 *  @brief Limpa a page corrente (não necessariamente a aparete)
 */
void erase(){
	cleardevice();
	setbkcolor(BGCOLOR);
}

/**
 *  @brief Faz a carga de uma imagem apartir do path do arquivo para a memória do programa
 *  
 *  @param [out] img_obj  Onde a imagem será estocada
 *  @param [in] caminho Path da imagem
 */
void graphInitObjects(graph_data_type *img_obj, const char* caminho, const char* caminhomsk){
	unsigned size;
	int left, top, right, bottom;
	setbkcolor(BGCOLOR);
	
	int page = getactivepage();
	setactivepage(2);
	
	cleardevice();
	left = (SCREEN_W/2)-(img_obj->w/2);
	top = (SCREEN_H/2)-(img_obj->h/2);
	right = (SCREEN_W/2)+(img_obj->w/2);
	bottom = (SCREEN_H/2)+(img_obj->h/2);	
	
	readimagefile(caminho,left, top, right, bottom );
	size= imagesize(left, top,right,bottom);
	img_obj->img = new int[size];
	getimage(left, top, right, bottom, img_obj->img);
	cleardevice();
	
	if(strcmp(caminhomsk," ")){
		readimagefile(caminhomsk,left, top, right, bottom );
		size= imagesize(left, top,right,bottom);
		img_obj->msk = new int[size];
		getimage(left, top, right, bottom, img_obj->msk);
		cleardevice();	
	}
	
	setactivepage(page);
}

/**
 *  @brief Faz a comutação entre a página de escrita com a página visual.
 *  
 */
void updateScreen(){
	int page = getactivepage();
	setvisualpage(page);
	
	page = page ? 0:1; //Troca o buffer de escrita/print
	setactivepage(page);
	erase();
}

/**
 *  @brief Define o tamanho da fonte da letras a serem impressas
 *  
 *  @param [in] char_size  tamanho da fonte
 */
void fontSize(int char_size){
		settextstyle(4,HORIZ_DIR, char_size);
}

/**
 *  @brief Brief
 *  
 *  @param [in] texto  Texto a ser impresso na tela
 *  @param [in] pos  Posição na qual ele deve ser impresso
 *  @return Return_Description
 *  
 *  @details Details
 */
void printTxt(char *texto, vetor2d_type pos){ 
	outtextxy(pos.x, pos.y,texto);	
}


