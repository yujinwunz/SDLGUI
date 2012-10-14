#pragma once
#include "control.h"
#include "Label.h"
#include "form.h"

#define _tickCol {0,0,0,255}

#define _checkSize 15
#define _cS _checkSize

class radioButtonButton:public control{
public:
	bool autoposition;
	bool checked;
	radioButtonButton(form *parent, int x = 0, int y = -1, int w = _cS, int h = _cS);

	virtual void draw(SDL_Surface *s);

};

static void radiotoggle(SDL_Event *e,void *p);
static void radioLtoggle(SDL_Event *e,void *p);
static void radioBtoggle(SDL_Event *e,void *p);


class radioButton:public form{
	string text;
public:
	radioButton *left, *right;	//used for linking purposes. Only one radiobutton in a list can do this.
	string getText();
	void setText(string text);
	bool checked;
	label *l;
	radioButtonButton *b;
	radioButton(form *parent, int x, int y, int w, int h, radioButton *link = NULL);
	void mouseEnter(SDL_Event *e);
	void mouseDown(SDL_Event *e);
	~radioButton();
	virtual void draw(SDL_Surface *s);

	//used to set only one button in a group to true.
	void clearLeft(SDL_Event *e);
	void clearRight(SDL_Event *e);
	void addToGroup(radioButton *r);
};
