#pragma once
#include "control.h"
#include "Button.h"
#include "misc.h"
#include "scrollButton.h"
#include "form.h"

static void VScrollUp(SDL_Event *e, void*p);
static void VScrollDown(SDL_Event *e, void*p);
static void VScrollScroll(SDL_Event *e, void*p);
static void VScrollDone(SDL_Event *e, void*p);

class VScroll:public form{
public:
	Button *lower, *higher;
	scrollButton *scroll;
	int value, minVal, maxVal;
	int virtualPosition;
	int getScrollPos(int val);
	int getScrollVal(int pos);
	VScroll(form* parent, int x, int y, int w, int h, int minVal, int maxVal, int startVal = (1<<30));
	void setValue(int val);
	void rePosition();
	~VScroll();
	virtual void draw(SDL_Surface *s);
	void setVirtualPosition(int p);
};
