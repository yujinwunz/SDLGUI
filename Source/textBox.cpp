#include "textBox.h"
string textBox::getText(){
	return text;
}
void textBox::setText(string text){
	this->text = text;
}
textBox::textBox(form* parent, int x, int y, int w, int h):control(parent,x,y,w,h){
	text = "";
	addHandler(event_keydown,textBoxType);
	state = 0; cursor = 0; pan = 0;
	textColour.r = 0, textColour.g = 0, textColour.b = 50;
}

void textBox::draw(SDL_Surface *s){
	int textW, textH, cursorW;
	//show cursor on box!
	while(true){
		if(cursor-pan<0) {
			pan = max(0,pan-_TPI); 
			state = 0;
			continue;
		}
		TTF_SizeText(font,text.substr(pan,cursor-pan).c_str(),&cursorW,&height);
		if(cursorW>=width){
			pan = min(text.size(), pan+_TPI);
			state = 0;
			continue;
		}
		break;
	}
	SDL_FillRect(s,NULL,((_CR<<16)|(_CG<<8)|(_CB)));
	TTF_SizeText(font,text.substr(pan,text.size()-pan).c_str(),&textW,&textH);
		
	drawText(text.substr(pan,text.size()-pan),textW>>1,height>>1,font,s,
		textColour.r,textColour.g,textColour.b,Centered);
	//flash.
	if(focused){
		if(!((state>>5)&1)){
			//draw a line 1 pixel wide
			for(int i = 2; i < height-2; i++){
				drawPixel(s,cursorW,i,0,0,0);
			}
		}
		state++;
	}
}
void textBox::decrimentCursor(){
	if(cursor-1>=0) cursor--;
}
void textBox::incrimentCursor(){
	if(cursor+1<=text.size()) cursor++;
}
void textBox::backspace(){
	if(cursor-1>=0){
		text = text.substr(0,cursor-1)+text.substr(cursor,text.size()-cursor);
		cursor--;
	}
}
void textBox::pressDelete(){
	if(cursor+1<=text.size()) text = text.substr(0,cursor)+text.substr(cursor+1,text.size()-cursor);
}

void textBoxType(SDL_Event *e, void *p){
	textBox *s = (textBox*)p;
	//get the character.
	if(_DEGUGG)cout<<s->cursor<<"\n";;
	char c = 0; int key = e->key.keysym.sym;
	string text = s->getText();
	if(key>=SDLK_a&&key<=SDLK_z){
		//alphabet
		c = key-SDLK_a+'a';
	}else if(key>=SDLK_0&&key<=SDLK_9){
		//digit.
		c = key-SDLK_0+'0';
	}else if(key==SDLK_BACKSPACE){
		//backspace. special.
		s->backspace();
	}else if(key==SDLK_DELETE){
		//delete. special.
		s->pressDelete();
	}else if(key==SDLK_LEFT){
		s->decrimentCursor();
	}else if(key==SDLK_RIGHT){
		s->incrimentCursor();
	}
	if(c!=0){
		text = text.substr(0,s->cursor)+c+text.substr(s->cursor,text.size()-s->cursor);
		s->cursor++;
		s->setText(text);
	}
}