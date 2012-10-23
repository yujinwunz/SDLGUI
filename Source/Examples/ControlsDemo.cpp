#include "SDLGUI.h"
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

void quitProgram(SDL_Event *e, void *sender);	//foreward declaration for event delegates.
void HAdjust(SDL_Event *e, void *sender);	
void VAdjust(SDL_Event *e, void *sender);	

static bool running = true;

int main(){
	init();
	SDL_Surface *screen = screen = SDL_SetVideoMode(700,300,32,SDL_DOUBLEBUF|SDL_HWSURFACE);
	//we will create a form, and add some controls to it.
	form mainForm(0,0,700,300);

	//some buttons
	Button aButton(&mainForm,0,0,140,30);	//it is important to specify mainForm as aButton's parent
	Button Quit(&mainForm,150,0,100,30);

	//let's add some text to the buttons.
	aButton.text = "A button!"; Quit.text = "Quit";
	//lets attach an event delegate to Quit's left-click event.
	Quit.addHandler(event_Lclick,quitProgram);	//now, when we click "Quit", quitprogram will run.

	//this is a demo!
	HScroll hsDemo(&mainForm,300,0,200,30,0,100,0);
	VScroll vsDemo(&mainForm,300,30,30,200,0,100,50);

	textBox txDemo(&mainForm,600,10,100,30);	txDemo.setText("Hello world");
	label lbDemo(&mainForm,600,60,100,30);

	//controls have tags; void* pointers that can store info for any use.
	//In this demo, delegates HAdjust and VAdjust will access these tags to
	//locate the label "lbDemo", which they will modify.
	hsDemo.tag = vsDemo.tag = (void*)&lbDemo;
	hsDemo.addHandler(event_scroll,HAdjust);
	vsDemo.addHandler(event_scroll,VAdjust);

	radioButton rbDemo1(&mainForm,0,100,150,30);
	radioButton rbDemo2(&mainForm,0,140,150,30,&rbDemo1);//you can specify another radio button for which this 
	radioButton rbDemo3(&mainForm,0,180,150,30,&rbDemo2);//onegroups to. Union-find will happen, dw I've got this.
	rbDemo1.setText("Radiobutton 1");
	rbDemo2.setText("Radiobutton 2");
	rbDemo3.setText("Radiobutton 3");

	//throw a checkbox for good measure
	checkBox cbDemo(&mainForm,0,250,150,30); cbDemo.setText("Checkbox");

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


void quitProgram(SDL_Event *e, void *sender){
	Button *s = (Button*)sender;	//this delegate is called by a button. Note that runtime errors
	//will occur if the programmer doesn't ensure this.
	running = false;
}
void HAdjust(SDL_Event *e, void *sender){
	HScroll *s = (HScroll*)sender;

	label *labelToModify = (label*)s->tag;	//remember the tag we set programmically? the tag points to
	//the label control in the demo.
	labelToModify->text = intToString(s->value);	//intToString is included in SDLGUI.
}


void VAdjust(SDL_Event *e, void *sender){
	VScroll *s = (VScroll*)sender;

	label *labelToModify = (label*)s->tag;
	labelToModify->text = intToString(s->value);
}
