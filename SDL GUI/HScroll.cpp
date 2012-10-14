#include "HScroll.h"


int HScroll::getScrollPos(int val){
	int space = width - lower->width - higher->width - max((width-2*height)/(2+(maxVal-minVal)/(width>>3)),10);
	return lower->width + ((val-minVal)*space)/(maxVal-minVal);
}

int HScroll::getScrollVal(int pos){
	pos-=lower->width;
	int space = width - lower->width - higher->width - scroll->width;
	return (pos*(maxVal-minVal))/space;
}

HScroll::HScroll(form* parent, int x, int y, int w, int h, int minVal, int maxVal, int startVal):form(x,y,w,h,parent){
	if(startVal == (1<<30)) startVal = minVal;
	value = startVal;

	this->minVal = minVal;
	this->maxVal = maxVal;
	//buttons will be square, and fixed by height.
	lower = new Button(this,0,0,h,h);
	lower->text = "<";
	higher = new Button(this,width-h,0,h,h);
	higher->text = ">";
	scroll = new scrollButton(this,getScrollPos(value),0,max((width-2*h)/(2+(maxVal-minVal)/(width>>3)),10),h);
	//scroll = new Button(this,0,0,10,10);
	addChild(lower); addChild(higher); addChild(scroll);

	lower->addHandler(event_Lclick,HScrollDown);
	higher->addHandler(event_Lclick,HScrollUp);
	scroll->addHandler(event_mousemotion,HScrollScroll);
	scroll->addHandler(event_mousebuttonup,HScrollDone);

	virtualPosition = scroll->left;
}

void HScroll::setValue(int val){
	value =	max( min(maxVal,val),minVal);
	SDL_Event e;
	e.type = event_scroll; e.user.code = value;
	callEvent(&e,e.type);
}

void HScroll::rePosition(){
	scroll->left = getScrollPos(value);
	virtualPosition = scroll->left;
}
HScroll::~HScroll(){
	delete lower, higher, scroll;
}

void HScroll::draw(SDL_Surface *s){
	SDL_FillRect(s,NULL,((_CR>>1)<<16)|((_CG>>1)<<8)|(_CB>>1));
		
}

void HScroll::setVirtualPosition(int p){
	virtualPosition = p;
	scroll->left = max(min(p,width-higher->width-scroll->width),lower->width);
	setValue(getScrollVal( scroll->left));
}


static void HScrollUp(SDL_Event *e, void*p){
	HScroll *s = (HScroll*)((Button*)p)->parent;
	s->setValue(s->value+1);
	s->rePosition();
}

static void HScrollDown(SDL_Event *e, void*p){
	HScroll *s = (HScroll*)((Button*)p)->parent;
	s->setValue(s->value-1);
	s->rePosition();
}

static void HScrollScroll(SDL_Event *e, void*p){
	HScroll *s = (HScroll*)((scrollButton*)p)->parent;
	//the mouse moved.
	if(s->leftDown){
		//we should modify position
		s->scroll->left += e->motion.xrel;
		//s->setValue(s->getScrollVal(s->scroll->left));
		s->setVirtualPosition(s->virtualPosition+e->motion.xrel);
	}
}

static void HScrollDone(SDL_Event *e, void*p){
	HScroll *s = (HScroll*)((scrollButton*)p)->parent;
	s->setValue(s->getScrollVal(s->scroll->left));
	s->rePosition();
}