#include "textInput.h"

TextInput::TextInput(form* parent, int x, int y, int w, int h, string message):form(x,y,w,h,parent){
	title = new label(this,0,30,width,30); title->text = message;
	text = new textBox(this,10,70,width-20,30);	
	this->message = message;	
	Accept = new Button(this,width-100,110,90,40);	Accept->text = "Done";
	Cancel = new Button(this,width-200,110,90,40);	Cancel->text = "Cancel";

	addHandler(event_losefocus,close);
}

TextInput::~TextInput(){
	delete title, Accept, Cancel, text;
}

void close(SDL_Event *e, void *p){
	TextInput *s = (TextInput*)p;
	s->parent->removeChild(s);
	s->callEvent(e,event_cancel);
	delete s;
}