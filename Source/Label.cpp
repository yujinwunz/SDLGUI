#include "Label.h"
label::label(form* parent,int x, int y, int w, int h):control(parent,x,y,w,h){

}
void label::draw(SDL_Surface *s){
	drawText(text,width>>1,height>>1,font,s,
		textColour.r,textColour.g,textColour.b,Centered);
}