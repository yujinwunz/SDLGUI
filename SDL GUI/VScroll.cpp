#include "VScroll.h"

int VScroll::getScrollPos(int val){
	int space = height - lower->height - higher->height - max((height-2*width)/(2+(maxVal-minVal)/(height>>3)),10);
	return lower->height + ((val-minVal)*space)/(maxVal-minVal);
}

int  VScroll::getScrollVal(int pos){
	pos-=lower->height;
	int space = height - lower->height - higher->height - scroll->height;
	return (pos*(maxVal-minVal))/space;
}

 VScroll::VScroll(form* parent, int x, int y, int w, int h, int minVal, int maxVal, int startVal):form(x,y,w,h,parent){
	if(startVal == (1<<30)) startVal = minVal;
	value = startVal;

	this->minVal = minVal;
	this->maxVal = maxVal;
	//buttons will be square, and fixed by height.
	lower = new Button(this,0,0,w,w);
	lower->text = "^";
	higher = new Button(this,0,height-w,w,w);
	higher->text = "v";
	scroll = new scrollButton(this,0,getScrollPos(value),w,max((height-2*w)/(2+(maxVal-minVal)/(height>>3)),10));
	//scroll = new Button(this,0,0,10,10);
	addChild(lower); addChild(higher); addChild(scroll);

	lower->addHandler(event_Lclick,VScrollDown);
	higher->addHandler(event_Lclick,VScrollUp);
	scroll->addHandler(event_mousemotion,VScrollScroll);
	scroll->addHandler(event_mousebuttonup,VScrollDone);

	virtualPosition = scroll->top;
}

void  VScroll::setValue(int val){
	value =	max( min(maxVal,val),minVal);
	SDL_Event e;
	e.type = event_scroll; e.user.code = value;
	callEvent(&e,e.type);
}

void  VScroll::rePosition(){
	scroll->top = getScrollPos(value);
	virtualPosition = scroll->top;
}
 VScroll::~VScroll(){
	delete lower, higher, scroll;
}

void  VScroll::draw(SDL_Surface *s){
	SDL_FillRect(s,NULL,((_CR>>1)<<16)|((_CG>>1)<<8)|(_CB>>1));
		
}

void  VScroll::setVirtualPosition(int p){
	virtualPosition = p;
	scroll->top = max(min(p,height-higher->height-scroll->height),lower->height);
	setValue(getScrollVal( scroll->top));
}


static void VScrollUp(SDL_Event *e, void*p){
	VScroll *s = (VScroll*)((Button*)p)->parent;
	s->setValue(s->value+1);
	s->rePosition();
}

static void VScrollDown(SDL_Event *e, void*p){
	VScroll *s = (VScroll*)((Button*)p)->parent;
	s->setValue(s->value-1);
	s->rePosition();
}

static void VScrollScroll(SDL_Event *e, void*p){
	VScroll *s = (VScroll*)((scrollButton*)p)->parent;
	//the mouse moved.
	if(s->leftDown){
		//we should modify position
		//s->setValue(s->getScrollVal(s->scroll->left));
		s->setVirtualPosition(s->virtualPosition+e->motion.yrel);
	}
}

static void VScrollDone(SDL_Event *e, void*p){
	VScroll *s = (VScroll*)((scrollButton*)p)->parent;
	s->setValue(s->getScrollVal(s->scroll->top));
	s->rePosition();
}