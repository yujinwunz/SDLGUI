#pragma once
#define _TextPanIncriment 7
#define _TPI _TextPanIncriment
#include "control.h"
#include "misc.h"

void textBoxType(SDL_Event *e, void *p);

class textBox: public control{
	string text;
public:
	string getText();
	void setText(string text);
	textBox(form* parent, int x, int y, int w, int h);
	//rendering variables!
	int state, cursor, pan;
	virtual void draw(SDL_Surface *s);
	void decrimentCursor();
	void incrimentCursor();
	void backspace();
	void pressDelete();
};

