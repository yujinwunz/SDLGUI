#include "SDLGUI.h"
#undef main
using namespace std;



TTF_Font *DefaultFont;
SDL_Surface *screen;

#define MAIN_INCLUDED

bool init(){
	if(SDL_Init(SDL_INIT_EVERYTHING)==-1 || TTF_Init() == -1){
		cout<<"Error initializing.\n";
		return true;
	};
	SDL_EnableKeyRepeat(500,25);
	//make default font ariel.
	DefaultFont = TTF_OpenFont("C:/Windows/Fonts/arial.ttf",16);
	return 0;
}


void main(){
	if(init()) return;
	screen = SDL_SetVideoMode(1200,700,32,SDL_DOUBLEBUF|SDL_HWSURFACE);
	
	
	system("pause");
}