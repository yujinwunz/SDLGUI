#pragma once
#ifndef _buttonCPP
#define _buttonCPP
#include "Button.h"

Button::Button(form* parent, int x, int y, int w, int h):control(parent,x,y,w,h){
	backColour.r = backColour.g = backColour.b = 100;
}

void Button::draw(SDL_Surface* s){
	if ((hovered&&!leftDown)||(leftDown&&!hovered)) SDL_FillRect(s,NULL,(foreColour.r<<16)|(foreColour.g<<8)|foreColour.b);
	else if ((leftDown&&hovered)||(!leftDown&&!hovered))SDL_FillRect(s,NULL,(backColour.r<<16)|(backColour.g<<8)|backColour.b);

	SDL_Color darkborder = {0,0,0,0}, lightborder = {170,170,170,255};
	if(hovered&&leftDown){
		SDL_Color temp(darkborder);
		darkborder = lightborder;
		lightborder = temp;
	}
	//draw a border _bBT thick.
	for(int top = 0; top < _bBT; top++) for(int p = top; p <width-top; p++)
		drawPixel(s,p,top,lightborder);
	for(int bottom = 0; bottom < _bBT; bottom++) for(int p = bottom; p <width-bottom; p++)
		drawPixel(s,p,height-bottom-1,darkborder);
	for(int left = 0; left < _bBT; left++) for(int p = left; p <height-left; p++)
		drawPixel(s,left,p,lightborder);
	for(int right = 0; right < _bBT; right++) for(int p = right; p <height-right; p++)
		drawPixel(s,width-right-1,p,darkborder);
	//draw focus box if focused.
	if(focused){
		for(int p = _bBT+1; p <width-_bBT-1; p+=2) drawPixel(s,p,_bBT+2,0,0,0);
		for(int p = _bBT+1; p <width-_bBT-1; p+=2) drawPixel(s,p,height-_bBT-2,0,0,0);
		for(int p = _bBT+1; p <height-_bBT-1; p+=2) drawPixel(s,_bBT+2,p,0,0,0);
		for(int p = _bBT+1; p <height-_bBT-1; p+=2) drawPixel(s,width-_bBT-2,p,darkborder);
	}
	//draw text
	drawText(text,width/2,height/2,font,s,textColour.r,textColour.g,
		textColour.b,Centered);
}
#endif