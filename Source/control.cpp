
#include "control.h"
#include "form.h"

TTF_Font *DefaultFont;

Uint32 timerTick(Uint32 interval, void* p){
	SDL_Event events[10];
	//check if there are one or more frame events. If so, don't tick another one!
	if(SDL_PeepEvents(events,10,SDL_PEEKEVENT,SDL_EVENTMASK(event_frameTick))>=1) return interval;
	//push an event.
	SDL_Event e;
	e.type = event_frameTick;
	e.user.data1 = p;
	SDL_PushEvent(&e);
	return interval;
}

void control::frameTick(SDL_Event *e){
	for(int i = 0; i < children.size(); i++) children[i]->frameTick(e);
	callEvent(e,event_frameTick);
}

void control::addHandler(int type, void (*h)(SDL_Event*,void*)){
	handler[type] = h;
}

void control::mouseEnter(SDL_Event* e){
	if(!hovered){
		hovered = true;
		callEvent(e,event_enter);
	}
}
void control::mouseLeave(SDL_Event* e){
	if(hovered){
		hovered = false;
		callEvent(e,event_leave);
	}
}
void control::mouseDown(SDL_Event *e){
	if(e->button.button == SDL_BUTTON_LEFT) leftDown = true;
	if(e->button.button == SDL_BUTTON_RIGHT) rightDown = true;
	if(e->button.button == SDL_BUTTON_MIDDLE) middleDown = true;
}
void control::mouseUp(SDL_Event *e){
	if(e->button.button == SDL_BUTTON_LEFT) leftDown = false;
	if(e->button.button == SDL_BUTTON_RIGHT) rightDown = false;
	if(e->button.button == SDL_BUTTON_MIDDLE) middleDown = false;
}
void control::draw(SDL_Surface *s){
	SDL_FillRect(s,NULL,(foreColour.r<<16)|(foreColour.g<<8)|foreColour.b);

}	//the default draw.

void control::callEvent(SDL_Event *e, unsigned short type){
	if(handler[type] != NULL) (handler[type])(e,this);
}

control::control(form *parent, SDL_Colour *foreColour, SDL_Colour *backColour, SDL_Color *textColour, TTF_Font *font){
	SDL_Colour foreC = _defaultForecolour;
	SDL_Colour backC = _defaultBackcolour;
	SDL_Colour textC = _defaultTextcolour;
	TTF_Font *dFont = _defaultFont;
	this->parent=NULL;
	setParent(parent);
	if(foreColour != NULL) this->foreColour = *foreColour;
	else this->foreColour = foreC;
	if(backColour != NULL) this->backColour = *foreColour;
	else this->backColour = backC;
	if(textColour != NULL) this->textColour = *foreColour;
	else this->textColour = textC;
	if(font != NULL) this->font = font;
	else this->font = dFont;
		
	hovered=leftDown=middleDown=rightDown=focused=false;
	for(int i = 0; i < 256; i++) this->handler[i] = NULL;

	enabled = true;
}

control::control(form *parent, int x, int y, int width, int height, SDL_Colour *foreColour, SDL_Colour *backColour, SDL_Color *textColour, TTF_Font *font){
	this->left = x; this->top = y; this->width = width; this->height = height;
		
	SDL_Colour foreC = _defaultForecolour;
	SDL_Colour backC = _defaultBackcolour;
	SDL_Colour textC = _defaultTextcolour;
	TTF_Font *dFont = _defaultFont;

	this->parent = NULL;
	setParent(parent);
	if(foreColour != NULL) this->foreColour = *foreColour;
	else this->foreColour = foreC;
	if(backColour != NULL) this->backColour = *foreColour;
	else this->backColour = backC;
	if(textColour != NULL) this->textColour = *foreColour;
	else this->textColour = textC;
	if(font != NULL) this->font = font;
	else this->font = dFont;
		
	hovered=leftDown=middleDown=rightDown=focused=false;
	for(int i = 0; i < 256; i++) this->handler[i] = NULL;

	enabled = true;
}

void control::getFocus(){
	focused = true;
}

void control::loseFocus(){
	focused = false;
}

void control::render(SDL_Surface* s){
	SDL_Surface *ts = SDL_CreateRGBSurface(SDL_SWSURFACE,width,height, 32,255<<16,255<<8,255,0);

	SDL_Event e;
	e.type = event_render;
	e.user.data1 = ts;

	SDL_Rect dRect = {left,top,width,height};
	SDL_BlitSurface(s,&dRect,ts,NULL);
	if(handler[event_render]!=NULL)callEvent(&e,event_render);
	else draw(ts);

	//for(int i = 0; i < children.size(); i++){
	//	children[i]->render(ts);
	//}
	//test
	//drawPixel(ts,1,1,255,255,255);
	//end test
		
	SDL_BlitSurface(ts,NULL,s,&dRect);
	SDL_FreeSurface(ts);
}

void control::setPos(int x, int y){
	this->left = x; this->top = y;
}
void control::setSize(int x, int y){
	this->width = x; this->height = y;
}
void control::setForeColour(int r, int g, int b, int a){
	SDL_Color c = {r,g,b,a};
	foreColour = c;
}
void control::setBackColour(int r, int g, int b, int a){
	SDL_Color c = {r,g,b,a};
	backColour = c;
}
void control::setTextColour(int r, int g, int b, int a){
	//cout<<r<<","<<g<<","<<b<<"\n";
	SDL_Color c = {r,g,b,a};
	textColour = c;
}
void control::setFont(string family, int size){
	//font = TTF_OpenFont(family.c_str(),size);
}
void control::setFont(TTF_Font* f){
	//font = TTF_OpenFont(family.c_str(),size);
	font = f;
}
void control::setParent(form* p){
	if(p==parent) return;
	if(p!=NULL) p->addChild(this);
	parent = p;
}

void control::handleEvent(SDL_Event &e){
	callEvent(&e,e.type);
}
