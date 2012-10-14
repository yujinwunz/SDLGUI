#pragma once
#include "control.h"

class label:public control{
public:
	string text;
	label(form* parent,int x, int y, int w, int h);
	virtual void draw(SDL_Surface *s);
};