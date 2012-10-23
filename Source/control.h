#pragma once
#ifndef _ControlInit
#define _ControlInit
#define _DEGUGG false
#define _BACKCOLOUR 0
//#include "misc.cpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <Windows.h>
#include "misc.h"

using namespace std;
class form;

#define _defaultForecolour {100,100,50,255}
#define _defaultBackcolour {10,10,10,255}
#define _defaultTextcolour {255,255,200,255}

#define _defaultFont DefaultFont
bool within(int x, int y, int w, int h, int x1, int y1);

#define _controlOutlineR 20
#define _controlOutlineG 20
#define _controlOutlineB 20
#define _COR _controlOutlineR
#define _COG _controlOutlineG
#define _COB _controlOutlineB
#define _controlR 200
#define _controlG 200
#define _controlB 200
#define _CR _controlR
#define _CG _controlG
#define _CB _controlB


enum EventID{

	event_noevent = SDL_NOEVENT ,
	event_activeevent = SDL_ACTIVEEVENT ,
	event_keydown = SDL_KEYDOWN ,
	event_keyup = SDL_KEYUP ,
	event_mousemotion = SDL_MOUSEMOTION ,
	event_mousebuttondown = SDL_MOUSEBUTTONDOWN ,
	event_mousebuttonup = SDL_MOUSEBUTTONUP ,
	event_joyaxismotion = SDL_JOYAXISMOTION ,
	event_joyballmotion = SDL_JOYBALLMOTION ,
	event_joyhatmotion = SDL_JOYHATMOTION ,
	event_joybuttondown = SDL_JOYBUTTONDOWN ,
	event_joybuttonup = SDL_JOYBUTTONUP ,
	event_quit = SDL_QUIT ,
	event_syswmevent = SDL_SYSWMEVENT ,
	event_eventreserveda = SDL_EVENT_RESERVEDA ,
	event_eventreservedb = SDL_EVENT_RESERVEDB ,
	event_videoresize = SDL_VIDEORESIZE ,
	event_videoexpose = SDL_VIDEOEXPOSE ,
	event_enter = 70,
	event_leave = 71,
	event_gainfocus = 72,
	event_losefocus = 73,
	event_Lclick = 74,
	event_Mclick = 75,
	event_Rclick = 76,
	event_render = 77,
	event_checkchanged = 78,
	event_scroll = 79,
	event_frameTick = 25,

	event_textChanged = 81,
	event_accept = 82,
	event_cancel = 83,
};
Uint32 timerTick(Uint32 interval, void* p);
class control{
public:
	bool enabled;
	virtual void frameTick(SDL_Event *e);
	string name;
	void addHandler(int type, void (*h)(SDL_Event*,void*));
	void (*handler[256])(SDL_Event*,void*);
	SDL_Colour foreColour;
	SDL_Colour backColour;
	SDL_Colour textColour;
	TTF_Font *font;

	bool hovered, leftDown, rightDown, middleDown, focused;
	virtual void mouseEnter(SDL_Event* e);
	virtual void mouseLeave(SDL_Event* e);
	virtual void mouseDown(SDL_Event *e);
	virtual void mouseUp(SDL_Event *e);
	virtual void draw(SDL_Surface *s);
	form *parent;
	void callEvent(SDL_Event *e, unsigned short type);
public:
	int left, top, width, height;
	control(form *parent, SDL_Colour *foreColour = NULL, SDL_Colour *backColour = NULL, SDL_Color *textColour = NULL, TTF_Font *font = NULL);

	control(form *parent, int x, int y, int width, int height, SDL_Colour *foreColour = NULL, SDL_Colour *backColour = NULL, SDL_Color *textColour = NULL, TTF_Font *font = NULL);
	virtual void getFocus();
	virtual void loseFocus();
	vector <control*> children;
	virtual void render(SDL_Surface* s);

	void *tag;

	virtual void setPos(int x, int y);
	virtual void setSize(int x, int y);
	virtual void setForeColour(int r, int g, int b, int a = 255);
	virtual void setBackColour(int r, int g, int b, int a = 255);
	virtual void setTextColour(int r, int g, int b, int a = 255);
	virtual void setFont(string family, int size);
	virtual void setFont(TTF_Font *font);
	virtual void setParent(form* p);
	virtual void handleEvent(SDL_Event &e);
};


#endif