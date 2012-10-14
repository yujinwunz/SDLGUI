#pragma once
#ifndef _buttonInit
#define _buttonInit
#include "control.h"

#define  _buttonBorderThickness 2
#define _bBT _buttonBorderThickness

class Button :public control{
public:
	Button(form* parent, int x=10, int y=10, int w=60, int h=20);
	string text;
	virtual void draw(SDL_Surface* s);
};
#endif


