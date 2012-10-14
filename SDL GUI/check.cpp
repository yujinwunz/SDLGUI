#include "check.h"

checkBoxBox::checkBoxBox(form *parent, int x, int y, int w, int h):control(parent,x,y,w,h){
	if(y<0) autoposition = true;
	else autoposition = false;
	checked = false;
}

void checkBoxBox::draw(SDL_Surface *s){
	if((hovered&&!leftDown)||(!hovered&&leftDown)){
		SDL_FillRect(s,NULL,(_COR<<16)|(_COG<<8)|(_COB));
		SDL_Rect r = {1,1,width-2,height-2};
		SDL_FillRect(s,&r,(_CR<<16)|(_CG<<8)|_CB);
	}else{
		SDL_FillRect(s,NULL,(_COR<<16)|(_COG<<8)|(_COB));
		SDL_Rect r = {1,1,width-2,height-2};
		SDL_FillRect(s,&r,((_CR>>1)<<16)|((_CG>>1)<<8)|(_CB>>1));
	}
	if(checked){
		//draw cross
		SDL_Color col = _tickCol;
		for(int i = 2; i < width-2; i++){
			drawPixel(s,i,i,col);
			drawPixel(s,-i,i,col);
		}
	}

	if(autoposition && parent!=NULL) top = (parent->height-height)>>1;
}

string checkBox::getText(){
	return l->text;
}
void checkBox::setText(string text){
	l->text = text;
}

checkBox::checkBox(form *parent, int x, int y, int w, int h):form(x,y,w,h,parent){
	checked = false;

	l = new label(this,10+_cS,0,w-_cS-10,h);
	b = new checkBoxBox(this,0,-1,_cS,_cS);

	l->addHandler(event_Lclick,&Ltoggle);
	b->addHandler(event_Lclick,&Btoggle);
	addHandler(event_Lclick,&toggle);
	addChild(l);
	addChild(b);
	backColour.r = backColour.g = backColour.b = 100;
	foreColour.r = foreColour.g = foreColour.b = 100;
	if(_DEGUGG)cout<<"Init\n";
}
void checkBox::mouseEnter(SDL_Event *e){
	if(_DEGUGG)cout<<"Entered\n";
}
void checkBox::mouseDown(SDL_Event *e){
	if(_DEGUGG)cout<<"~Downed\n";
}
checkBox::~checkBox(){
	delete l; delete b;
}
void checkBox::draw(SDL_Surface *s){};

static void toggle(SDL_Event *e, void* p){
	checkBox *s = (checkBox*)(p);
	s->b->checked = s->checked = !s->checked;
	s->callEvent(e,event_checkchanged);
	if(_DEGUGG)cout<<"Toggled: "<<s->checked<<","<<s->b->checked<<"\n";
}

static void Btoggle(SDL_Event *e, void* p){
	checkBoxBox *s = (checkBoxBox*)(p);
	toggle(e,s->parent);
}

static void Ltoggle(SDL_Event *e, void* p){
	label *s = (label*)(p);
	toggle(e,s->parent);
}

