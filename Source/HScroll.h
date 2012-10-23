#pragma once
#include "control.h"
#include "Button.h"
#include "misc.h"
#include "scrollButton.h"
#include "form.h"

static void HScrollUp(SDL_Event *e, void*p);
static void HScrollDown(SDL_Event *e, void*p);
static void HScrollScroll(SDL_Event *e, void*p);
static void HScrollDone(SDL_Event *e, void*p);

class HScroll:public form{
public:
	Button *lower, *higher;
	scrollButton *scroll;
	int value, minVal, maxVal;
	int virtualPosition;
	int getScrollPos(int val);

	int getScrollVal(int pos);

	HScroll(form* parent, int x, int y, int w, int h, int minVal, int maxVal, int startVal = (1<<30));

	void setValue(int val);

	void rePosition();
	~HScroll();

	virtual void draw(SDL_Surface *s);

	void setVirtualPosition(int p);
};
