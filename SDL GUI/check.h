#pragma once
#include "control.h"
#include "form.h"
#include "Label.h"


#define _tickCol {0,0,0,255}

#define _checkSize 15
#define _cS _checkSize
#include "misc.h"
class checkBoxBox:public control{
public:
	bool autoposition;
	bool checked;
	checkBoxBox(form *parent, int x = 0, int y = -1, int w = _cS, int h = _cS);
	virtual void draw(SDL_Surface *s);
};

static void toggle(SDL_Event *e,void *p);
static void Ltoggle(SDL_Event *e,void *p);
static void Btoggle(SDL_Event *e,void *p);
static void down(SDL_Event *e,void *p);


class checkBox:public form{
	string text;
public:
	string getText();
	void setText(string text);
	bool checked;
	label *l;
	checkBoxBox *b;
	checkBox(form *parent, int x, int y, int w, int h);
	void mouseEnter(SDL_Event *e);
	void mouseDown(SDL_Event *e);
	~checkBox();
	virtual void draw(SDL_Surface *s);
};
