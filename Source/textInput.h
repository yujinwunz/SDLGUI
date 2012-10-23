#pragma once
#include "control.h"
#include "form.h"
#include "Button.h"
#include "textBox.h"
#include "Label.h"

class TextInput:public form{
public:
	string message;
	label *title;
	Button *Accept, *Cancel;
	textBox *text;
	TextInput(form* parent, int x, int y, int w, int h, string message);
	~TextInput();
};

void close(SDL_Event *e, void *p);