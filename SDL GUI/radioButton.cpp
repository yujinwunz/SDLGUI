#include "radiobutton.h"
radioButtonButton::radioButtonButton(form *parent, int x, int y, int w, int h):control(parent,x,y,w,h){
	if(y<0) autoposition = true;
	else autoposition = false;
	checked = false;
}

void radioButtonButton::draw(SDL_Surface *s){
	fillCircle(s,width>>1,height>>1,height>>1,_CR,_CG,_CB);
	DrawCircle(s,width>>1,height>>1,height>>1,_COR,_COG,_COB);
		
	if(checked){
		//draw dot
		SDL_Color col = _tickCol;
		fillCircle(s,width>>1,height>>1,height>>2,col);
	}

	if(autoposition && parent!=NULL) top = (parent->height-height)>>1;
}

string radioButton::getText(){
	return l->text;
}
void radioButton::setText(string text){
	l->text = text;
}

radioButton::radioButton(form *parent, int x, int y, int w, int h, radioButton *link ):form(x,y,w,h,parent){
	checked = false;

	l = new label(this,10+_cS,0,w-_cS-10,h);
	b = new radioButtonButton(this,0,-1,_cS,_cS);

	l->addHandler(event_Lclick,&radioLtoggle);
	b->addHandler(event_Lclick,&radioBtoggle);
	addHandler(event_Lclick,&radiotoggle);
	addChild(l);
	addChild(b);
	backColour.r = backColour.g = backColour.b = 100;
	foreColour.r = foreColour.g = foreColour.b = 100;
	if(_DEGUGG) cout<<"Init\n";
	left = right = NULL;
	if(link!=NULL) link->addToGroup(this);
}
void radioButton::mouseEnter(SDL_Event *e){
	if(_DEGUGG) cout<<"Entered\n";
}
void radioButton::mouseDown(SDL_Event *e){
	if(_DEGUGG) cout<<"~Downed\n";
}
radioButton::~radioButton(){
	delete l; delete b;
	if(left!=NULL) left->right = right;
	if(right!=NULL) right->left = left;
}
void radioButton::draw(SDL_Surface *s){};

//used to set only one button in a group to true.
void radioButton::clearLeft(SDL_Event *e){
	if(checked) callEvent(e,event_checkchanged);
	checked = b->checked = false;
	callEvent(e,event_checkchanged);
	if(left!=NULL) left->clearLeft(e);
}

void radioButton::clearRight(SDL_Event *e){
	if(checked) callEvent(e,event_checkchanged);
	checked = b->checked = false;
	callEvent(e,event_checkchanged);
	if(right != NULL) right->clearRight(e);
}

void radioButton::addToGroup(radioButton *r){
	if(right==NULL){
		r->left = this;
		this->right = r;
	}else{
		right->addToGroup(r);
	}
}


static void radiotoggle(SDL_Event *e, void* p){
	//not really toggle. Sets to true, and does that thing.
	radioButton *s = (radioButton*)(p);
	if(s->checked == false){
		s->b->checked = s->checked = true;
		s->callEvent(e,event_checkchanged);
	}
	s->b->checked = s->checked = true;
	if(s->left!=NULL)s->left->clearLeft(e);
	if(s->right!=NULL)s->right->clearRight(e);
}

static void radioBtoggle(SDL_Event *e, void* p){
	radioButtonButton *s = (radioButtonButton*)(p);
	radiotoggle(e,s->parent);
}

static void radioLtoggle(SDL_Event *e, void* p){
	label *s = (label*)(p);
	radiotoggle(e,s->parent);
}

