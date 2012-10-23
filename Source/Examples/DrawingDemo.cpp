#include "SDLGUI.h"
#include "misc.h"
#undef main
using namespace std;

extern TTF_Font *DefaultFont;
SDL_Surface *screen;

bool init(){	//This is the initializer called by main which sets up the environment for SDL and truetype fonts.
	if(SDL_Init(SDL_INIT_EVERYTHING)==-1 || TTF_Init() == -1){
		cout<<"Error initializing.\n";
		return true;
	};
	SDL_EnableKeyRepeat(500,25);
	//make default font a free font
	DefaultFont = TTF_OpenFont("LiberationSans-Bold.ttf",16);	//Decides on a default font.
	return 0;
}

static bool running = true;

void ColourfulDraw(SDL_Event *e, void* p);	//our new dawing function which will effectively override the stock one
void ChangeRed(SDL_Event *e, void* p);
void ChangeBlue(SDL_Event *e, void* p);
void ChangeGreen(SDL_Event *e, void* p);
void Quit(SDL_Event *e, void* p);


int main(){
	init();
	SDL_Surface *screen = screen = SDL_SetVideoMode(500,400,32,SDL_DOUBLEBUF|SDL_HWSURFACE);
	//we will create a form.
	form mainForm(0,0,500,400);

	//in this demo, there will be three scroll bars that control the colour of a
	//button. The button will also host its own drawing function, called by an
	//event_render.

	Button ColourfulQuit(&mainForm,100,250,150,50); ColourfulQuit.text = "Quit with style";

	HScroll Red(&mainForm,100,100,200,30,0,255,100); Red.tag = &ColourfulQuit;
	HScroll Green(&mainForm,100,150,200,30,0,255,100); Green.tag = &ColourfulQuit;
	HScroll Blue(&mainForm,100,200,200,30,0,255,255); Blue.tag = &ColourfulQuit;

	//add some handlers
	Red.addHandler(event_scroll,ChangeRed);
	Green.addHandler(event_scroll,ChangeGreen);
	Blue.addHandler(event_scroll,ChangeBlue);

	ColourfulQuit.addHandler(event_Lclick,Quit);
	ColourfulQuit.addHandler(event_render,ColourfulDraw);

	//To make things work, we need to activate the form we created, "mainform".
	//All the event-driven craziness from SDL is handled in form.wait()
	//form.wait() will wait for exactly ONE event before returning execution.
	//you also need to set up a framerate, telling the form which screen to 
	//draw on. Every frame, an event_frame event is fired.
	mainForm.startFramerate(60,screen);
	while(running){
		mainForm.wait();
		SDL_Flip(screen);	//wait() will render to the screen but not flip, providing flexibility.
	}
	cout<<"Demo done\n";
	return 0;
}


void ColourfulDraw(SDL_Event *e, void* p){
	Button *s = (Button*)p;
	SDL_Surface *screen = (SDL_Surface*)e->user.data1;	//the surface is passed in the event, located ambiguously in data1. I might make this work better, I might not. Probably not.
	int c=0;
	if(s->leftDown&&s->hovered) c = ((s->foreColour.r>>1)<<16)|((s->foreColour.g>>1)<<8)|(s->foreColour.b>>1);
	else if(s->leftDown&&!s->hovered) c = (s->foreColour.r<<16)|(s->foreColour.g<<8)|s->foreColour.b;
	else if(!s->leftDown&&s->hovered) c = ((255-((255-s->foreColour.r)>>1))<<16)|((255-((255-s->foreColour.g)>>1))<<8)|((255-((255-s->foreColour.b)>>1))<<0);
	else c = (s->foreColour.r<<16)|(s->foreColour.g<<8)|s->foreColour.b;

	//lets leave the corners empty for some effect
	SDL_Rect r1 = {5,0,s->width-10,s->height};
	SDL_Rect r2 = {0,5,s->width,s->height-10};
	SDL_FillRect(screen,&r1,c);
	SDL_FillRect(screen,&r2,c);
	//text
	drawText(s->text,s->width/2,s->height/2,s->font,screen,s->textColour.r,s->textColour.g,s->textColour.b,Centered);
}
void ChangeRed(SDL_Event *e, void* p){
	HScroll *s = (HScroll*)p;
	Button *b = (Button*)s->tag;

	b->parent->foreColour.r = s->value>>1;
	b->foreColour.r = s->value;
}
void ChangeBlue(SDL_Event *e, void* p){
	HScroll *s = (HScroll*)p;
	Button *b = (Button*)s->tag;

	b->parent->foreColour.b = s->value>>1;
	b->foreColour.b = s->value;
}
void ChangeGreen(SDL_Event *e, void* p){
	HScroll *s = (HScroll*)p;
	Button *b = (Button*)s->tag;

	b->parent->foreColour.g = s->value>>1;
	b->foreColour.g = s->value;
}
void Quit(SDL_Event *e, void* p){
	running  = false;
}
