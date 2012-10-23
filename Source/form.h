#pragma once
#include "control.h"

class form:public control{
public:
	control *pressedControl;
	int findChild(control *c);
	int focusedControl;
	int hoveredControl;
public:
	form(int x = 0, int y = 0, int w = 100, int h = 100,form* parent=NULL);
	void addChild(control *c);
	void removeChild(control *c);
	int leftBindedControl, rightBindedControl, middleBindedControl;	//-1 means me. -2 means none.
	bool renderStatred;

	void handleMouseMove(SDL_Event &e);

	int prevMouseX, prevMouseY;
	void handleEvent(SDL_Event &e);
	virtual void mouseLeave(SDL_Event *e);
	void wait();	//the jist of event driven! Is responsible for event behavior of all controls, and especially the click event.
	virtual void loseFocus();
	//override the render.
	void render(SDL_Surface *s);
	float framerate;
	SDL_TimerID frametimer;
	void startFramerate(float fps, SDL_Surface *s);
	void stopFramerate();
	~form();
};