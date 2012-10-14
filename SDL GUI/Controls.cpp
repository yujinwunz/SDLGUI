/*#ifndef CONTROLS_INIT
#define CONTROLS_INIT

#include <iostream>
#include <fstream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <list>
using namespace std;

enum EventID{

	event_noevent = SDL_NOEVENT ,
	event_activeevent = SDL_ACTIVEEVENT ,
	event_keydown = SDL_KEYDOWN ,
	event_keyup = SDL_KEYUP ,
	event_mousemotion = SDL_MOUSEMOTION ,
	event_mousebuttondown = SDL_MOUSEBUTTONDOWN ,
	event_mousebuttonup = SDL_MOUSEBUTTONUP ,
	event_joyaxismotion = SDL_JOYAXISMOTION ,
	event_joyballmotion = SDL_JOYBALLMOTION ,
	event_joyhatmotion = SDL_JOYHATMOTION ,
	event_joybuttondown = SDL_JOYBUTTONDOWN ,
	event_joybuttonup = SDL_JOYBUTTONUP ,
	event_quit = SDL_QUIT ,
	event_syswmevent = SDL_SYSWMEVENT ,
	event_eventreserveda = SDL_EVENT_RESERVEDA ,
	event_eventreservedb = SDL_EVENT_RESERVEDB ,
	event_videoresize = SDL_VIDEORESIZE ,
	event_videoexpose = SDL_VIDEOEXPOSE ,
	event_enter = 70,
	event_leave = 71,
	event_gainfocus = 72,
	event_losefocus = 73,
	event_Lclick = 74,
	event_Mclick = 75,
	event_Rclick = 76,
	event_render = 77,
};

enum ControlState{
	Pressed,
	Hovered,
	Default,
	Entered,
};


class control{
public:
	
	bool clicked;

	int x, y, w, h;
	int r,g,b;

	list <control*> children;
	control* parent;

	//these are iterators to children. When deleting children, make sure these are set to end if something.
	list <control*>::iterator hoveredChild;

	ControlState state;
	bool focused;
	void (*handler[255])(control *sender, SDL_Event* e);

	void fire(int type, control * p1, SDL_Event *e){
		if(handler[type] != NULL) (*handler[type])(p1,e);
	}



	virtual void render(SDL_Surface *screen, int r = -1, int g = -1, int b = -1){
		//check rendered state
		
		SDL_Surface *temp = SDL_CreateRGBSurface(SDL_HWSURFACE,w,h,32,
			255<<24,255<<16,255<<8,255);
		SDL_SetAlpha(temp,SDL_SRCALPHA,255);
		SDL_Rect Drect = {x,y,w,h};

		if(handler[event_render] == NULL){
			if(r==-1) r = this->r; if(g==-1) g= this->g; if(b==-1) b = this->b;
			//the default render.
		
			//this is where we draw ourselves.
			renderSelf(temp);			

		}else{
			//user draw.
			SDL_Event e;
			e.user.data1 = temp;
			(*handler[event_render])(this,&e);
		}
		//this is where we draw our children
		for(list<control*>::iterator i = children.begin(); i != children.end(); i++){
			(*i)->render(temp);
		}
		//and we put pallette on screen.
	
		SDL_BlitSurface(temp,NULL,screen,&Drect);
		SDL_FreeSurface(temp);
	}

	virtual void renderSelf(SDL_Surface *screen){
		SDL_Rect Srect = {0,0,w,h};
		//used to render 
		switch (state){
			case Pressed:
				break;
			case Hovered:
				//r = 255-(255-r)/2; b = 255-(255-b)/2; g = 255-(255-g)/2;	//there are two modes
				//r = min(255,r+50); g = min(255,g+50); b = min(255,b+50);
				break;
			case Default:
				break;
			default:
				break;
			}
		SDL_FillRect(screen,&Srect,(r<<24)|(g<<16)|(b<<8)|255);
	}

	control(control* parent, int x, int y, int w, int h, int r, int g, int b){
		this->x = x; this->y = y; this->w = w; this-> h = h;
		this->r = r; this->g = g; this->b = b;
		this->parent = parent;

		parent->addChild(this);
		state = Default;
		for(int i = 0; i < 255; i++) handler[i] = NULL;

		clicked = false;

		hoveredChild = children.end();
	}

	control(){
		r=g=b=0;
		x = y = w = h = 0;
		state = Default;
		parent = NULL;
		for(int i = 0; i < 255; i++) handler[i] = NULL;

		clicked = false;

		hoveredChild = children.end();
	}

	void enter(SDL_Event *e){
		if(SDL_GetMouseState(NULL,NULL)|SDL_BUTTON_LMASK|SDL_BUTTON_RMASK|SDL_BUTTON_MMASK);
		else clicked = false;
		//NOT event handler. The event capsule.
		//search children.
		bool found = false;
		for(list<control*>::iterator i = children.begin(); i != children.end();i++){
			int x = (*i)->x, y = (*i)->y, w = (*i)->w, h = (*i)->h;
				

			SDL_Event ee = *e;
			ee.motion.x -= x;
			ee.motion.y -= y;
			int x1 = e->motion.x, y1 = e->motion.y;

			if((x1>=x)+(x1>x+w)==1 && (y1>=y)+(y1>y+h) == 1){
				(*i)->enter(&ee);
				found = true;
				hoveredChild = i;
			}
			else{
				(*i)->leave(&ee);
			}
		}

		if(found) state = Default;
		else if(state==Default){
			//fire event.
			hoveredChild = children.end();
			if(clicked) state = Pressed;
			else state = Hovered;
			fire(e->type,this,e);
		}
	}

	void leave(SDL_Event *e){
		//tell hovered child we have left.
		if(hoveredChild != children.end()){
			(*hoveredChild)->leave(e);
			hoveredChild = children.end();
		}
		if(state!=Default){
			//fire event
			state = Default;
			fire(e->type,this,e);
		}
	}

	void addChild(control* child){
		children.push_front(child);
	}

	void removeChild(control* child){
		//check iterators
		bool HC = false;
		if((*hoveredChild)==child || hoveredChild == children.end()) HC = true;
		children.remove(child);

		if(HC) hoveredChild = children.end();
	}

	void buttonUp(SDL_Event *e){
		//special triggers to manage click events.
		for(list<control*>::iterator i = children.begin(); i != children.end();i++){
			int x = (*i)->x, y = (*i)->y, w = (*i)->w, h = (*i)->h;
			int x1 = e->motion.x, y1 = e->motion.y;
			SDL_Event ee = *e;
			ee.motion.x -= x;
			ee.motion.y -= y;
			//positions will be given relative to me.
			bool found = false;

			if((x1>=x)+(x1>x+w)==1 && (y1>=y)+(y1>y+h) == 1){
				(*i)->buttonUp(&ee);
				found = true;
			}

			fire(e->type,this,&ee);

			if(!found){
				//fire click event if me.
				if(clicked){
					if(e->button.button == SDL_BUTTON_LEFT) fire(event_Lclick,this,&ee);
					else if (e->button.button == SDL_BUTTON_RIGHT) fire(event_Rclick,this,&ee);
					else fire(event_Mclick,this,&ee);
				}
			}
			clicked = false;
			state = Hovered;
		}
	}

	void buttonDown(SDL_Event *e){
		//special triggers to manage click events.
		for(list<control*>::iterator i = children.begin(); i != children.end();i++){
			int x = (*i)->x, y = (*i)->y, w = (*i)->w, h = (*i)->h;
			int x1 = e->motion.x, y1 = e->motion.y;
			SDL_Event ee = *e;
			ee.motion.x -= x;
			ee.motion.y -= y;
			//positions will be given relative to me.
			bool found = false;

			if((x1>=x)+(x1>x+w)==1 && (y1>=y)+(y1>y+h) == 1){
				(*i)->buttonUp(&ee);
				found = true;
			}

			fire(e->type,this,&ee);

			if(!found){
				clicked = true;
			}

			state = Pressed;
		}
	}

};


class form:public control{
	//this class acts like a windows form and manages stuff. Inherits from control...
public:
	form(int x, int y, int w, int h):control(){
		this->x = x; this->y = y; this->w = w;this->h = h;
	}

	void wait(){
		//wait for events, and make them happen.
		SDL_Event e;
		SDL_WaitEvent(&e);
		//select which type of event
		switch (e.type){
		case SDL_MOUSEMOTION:{
			
			//make hover thing.
			//trigger for every one.
			for(list<control*>::iterator i = this->children.begin(); i != this->children.end();i++){
				int x = (*i)->x, y = (*i)->y, w = (*i)->w, h = (*i)->h;
				int x1 = e.motion.x, y1 = e.motion.y;
				SDL_Event ee = e;
				ee.motion.x -= x;
				ee.motion.y -= y;
				//positions will be given relative to me.
				if((x1>=x)+(x1>x+w)==1 && (y1>=y)+(y1>y+h) == 1){
					(*i)->enter(&ee);
				}
				else{
					(*i)->leave(&ee);
				}
			}
							 }break;
		case event_mousebuttondown:{
			//give event to controls.
			for(list<control*>::iterator i = this->children.begin(); i != this->children.end();i++){
				int x = (*i)->x, y = (*i)->y, w = (*i)->w, h = (*i)->h;
				int x1 = e.motion.x, y1 = e.motion.y;
				SDL_Event ee = e;
				ee.motion.x -= x;
				ee.motion.y -= y;
				//positions will be given relative to me.
				if((x1>=x)+(x1>x+w)==1 && (y1>=y)+(y1>y+h) == 1){
					(*i)->buttonUp(&ee);
				}
			}
								   }break;
		case event_mousebuttonup:{
			//Give event to controls.
			for(list<control*>::iterator i = this->children.begin(); i != this->children.end();i++){
				int x = (*i)->x, y = (*i)->y, w = (*i)->w, h = (*i)->h;
				int x1 = e.motion.x, y1 = e.motion.y;
				SDL_Event ee = e;
				ee.motion.x -= x;
				ee.motion.y -= y;
				//positions will be given relative to me.
				if((x1>=x)+(x1>x+w)==1 && (y1>=y)+(y1>y+h) == 1){
					(*i)->buttonDown(&ee);
				}
			}
								 }break;
		default:
			break;
		}
	}

	void render(SDL_Surface *screen){
		//draws itself on its location on "screen", and then returns.
		SDL_Surface *temp = SDL_CreateRGBSurface(SDL_HWSURFACE,this->w,this->h,32,
			255<<24,255<<16,255<<8,255);
		SDL_SetAlpha(temp,SDL_SRCALPHA,255);

		//this is where it renders itself
		SDL_FillRect(temp,NULL,(0<<24)|(0<<16)|(10<<8)|(255));
		//this is where it renders its children
		for(list<control*>::iterator i = children.begin(); i != children.end(); i++){
			(*i)->render(temp);
		}
		
		SDL_Rect Drect = {this->x,this->y,this->w,this->h};
		SDL_BlitSurface(temp,NULL,screen,&Drect);
		SDL_FreeSurface(temp);
	}
};

#endif*/