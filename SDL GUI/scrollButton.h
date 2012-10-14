#pragma once
#include "misc.h"
#include "control.h"
#include "Button.h"

class scrollButton:public Button{
public:
	scrollButton(form* parent, int x, int y, int w, int h);
	virtual void mouseLeave(SDL_Event* e);
};