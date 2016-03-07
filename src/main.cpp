#include <iostream>
#include <conio.h>
#include <string>
#include "bgi/graphics.h"
#include "h/types.h"
#include "graph/grafico.h"
#include "joystick/joystick.h"

#define TARGET_FRAME_RATE (1.0/120.0)
/** TODO Verificar se a API do XInput aceita mais que 4 controles.*/
#define MAX_NUM_JOYSTICK	4

// Protótipos -----------------------------
int initJoysticks (Joystick *joy[],int joy_vec_size);
void printJoysticks (Joystick *joy, int num);
// ----------------------------------------


int main (void){
	
	int ret = 0;
	int joystick_count = 0;
	Joystick *joystick_vec[MAX_NUM_JOYSTICK];
	
	initwindow(SCREEN_W,SCREEN_H);
	
	memset(joystick_vec,0,sizeof(*joystick_vec[MAX_NUM_JOYSTICK])); // garante um vetor limpo
	
	// Inicializa os controles
	joystick_count = initJoysticks(joystick_vec,MAX_NUM_JOYSTICK);
	if(!joystick_count){
		std::cout << "<WARNIG> Please, connect a controller and restart this demo" << std::endl;
		std::cout << "Press any key to exit" << std::endl;
		while(!kbhit());
		
		goto exit;
	}
	

	while(ret >= 0){
		
		if(kbhit()) goto exit;
			
		for(int i = 0; i < joystick_count; ++i)
			printJoysticks(joystick_vec[i],i);
		
		updateScreen();
	}
	
exit:
	
	// Evita memory leak
	for(int joynum = 0; joynum < MAX_NUM_JOYSTICK; ++joynum){
		if(joystick_vec[joynum] != NULL)
			delete joystick_vec[joynum];
	}
	
	std::cout << "<TRACE> END" << std::endl;
	return 0;
}


int initJoysticks (Joystick *joy[],int joy_vec_size){
	
	int num_joy;
	
	if(!(num_joy =Joystick::deviceCount())){
		std::cout << "<WARNIG> No joysticks connected!" << std::endl;
		return 0;	
	}
	
	std::cout << "<TRACE> "<< '[' << num_joy << ']' << " joystick(s) founded." << std::endl; 
	
	// Garante que mesmo que encontre mais controles, iniciará o máximo suportado nesse demo
	num_joy = (num_joy > joy_vec_size) ? joy_vec_size:num_joy;
	
	// Inicializa todos os joysticks encontrados
	for (int i =0; i < num_joy; ++i){
		
		try{
			joy[i] = new Joystick(i);
		}
		catch (std::bad_alloc& ba){
			std::cerr << "<ERROR< bad_alloc on [initJoysticks]" << ba.what() << std::endl;
			return 0;
		}
		
		joy[i]->init();
	}
	
	return num_joy;
}

void printJoysticks (Joystick *joy, int num){
	
	std::stringstream info;
	char tmp[300];
	int y_index = 0;
	int text_x_pos = 100 + (num * 400);
		
	fontSize(1);
	
	info << "   Joystick " << num << "\n";
	strcpy(tmp,info.str().c_str());
	printTxt(tmp,vetor2d_type{text_x_pos,0});
	++y_index;
	
	info.str( std::string() );
	
	// Analog Axis --------------------
	
	info << "- Axis X [" << joy->getAxisX() << "]" << "\n";
	strcpy(tmp,info.str().c_str());
	printTxt(tmp,vetor2d_type{text_x_pos,30});
	
	info.str( std::string() );
	
	info << "- Axis Y [" << joy->getAxisY() << "]" << "\n";
	strcpy(tmp,info.str().c_str());
	printTxt(tmp,vetor2d_type{text_x_pos,60});
	
	info.str( std::string() );
	
	// DPAD like   ------------------
	
	int dpad_like = joy->getAxisPosition();
	
	if(dpad_like & Joystick::UP)
		info << "UP ";
		
	if(dpad_like & Joystick::DOWN)
		info << "DOWN ";
		
	if(dpad_like & Joystick::RIGHT)
		info << "RIGHT ";
		
	if(dpad_like & Joystick::LEFT)
		info << "LEFT ";

	strcpy(tmp,info.str().c_str());
	printTxt(tmp,vetor2d_type{text_x_pos,90});
	
	info.str( std::string() );
	
	// Buttons -----------------------
	
	for(int i = Joystick::BUTTON_1; i <= Joystick::BUTTON_START; ++i){
		info << "- Button " << i + 1 << " [" << joy->getButton(i) << "]" << "\n";
		strcpy(tmp,info.str().c_str());
		printTxt(tmp,vetor2d_type{text_x_pos,120 + (i*30)});
				
		info.str( std::string() );
	}	
}
