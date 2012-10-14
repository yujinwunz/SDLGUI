#pragma once
#ifndef MISC_INCLUDED
#define MISC_INCLUDED
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <list>
#include <cmath>
using namespace std;

enum TextAlign{
	Centered = 0,
	TopLeft = 1,
};


void drawText(string text, int x, int y, TTF_Font *font, SDL_Surface* dest, int r, int g, int b ,TextAlign a);
void drawText(string text, int x, int y, string family, int size, SDL_Surface* dest, int r, int g, int b, TextAlign a = TopLeft);
void drawText(string text, int x, int y, char *family, int size, SDL_Surface* dest, int r, int g, int b, TextAlign a = TopLeft);

extern void drawPixel(SDL_Surface *s, int x, int y, int r, int g, int b);
extern void drawPixel(SDL_Surface *s, int x, int y, SDL_Color &c);
string intToString(int number);

void DrawCircle8(SDL_Surface *s, int x, int y, int cx, int cy, int r, int g, int b, int a );

void DrawCircle(SDL_Surface *s, int posx, int posy, int radius, int r, int g, int b, int a=255);
void DrawCircle(SDL_Surface *s, int posx, int posy, int radius, SDL_Color &c);
/*OOPS copypasta the wrong thing. SIGH. Write my own...
void FillCircle(SDL_Surface *s, int xCenter, int yCenter, int radius, int r, int g, int b, int a = 255){
    int x, y, r2;

    r2 = radius * radius;
    drawPixel(s, xCenter, yCenter + radius,r,g,b);
    drawPixel(s, xCenter, yCenter - radius,r,g,b);
    drawPixel(s, xCenter + radius, yCenter,r,g,b);
    drawPixel(s, xCenter - radius, yCenter,r,g,b);

    y = radius;
    x = 1;
    y = (int) (sqrt(r2 - 1) + 0.5);
    while (x < y) {
        drawPixel(s, xCenter + x, yCenter + y,r,g,b);
        drawPixel(s, xCenter + x, yCenter - y,r,g,b);
        drawPixel(s, xCenter - x, yCenter + y,r,g,b);
        drawPixel(s, xCenter - x, yCenter - y,r,g,b);
        drawPixel(s, xCenter + y, yCenter + x,r,g,b);
        drawPixel(s, xCenter + y, yCenter - x,r,g,b);
        drawPixel(s, xCenter - y, yCenter + x,r,g,b);
        drawPixel(s, xCenter - y, yCenter - x,r,g,b);
        x += 1;
        y = (int) (sqrt(r2 - x*x) + 0.5);
    }
    if (x == y) {
        drawPixel(s, xCenter + x, yCenter + y,r,g,b);
        drawPixel(s, xCenter + x, yCenter - y,r,g,b);
        drawPixel(s, xCenter - x, yCenter + y,r,g,b);
        drawPixel(s, xCenter - x, yCenter - y,r,g,b);
    }
}

void FillCircle(SDL_Surface *s, int xCenter, int yCenter, int radius, SDL_Color &c){
	FillCircle(s, xCenter, yCenter, radius, c.r, c.g, c.b, c.unused );
}*/

void fillCircle(SDL_Surface *s, int cx, int cy, int radius, int r, int g, int b, int a = 255);
void fillCircle(SDL_Surface *s, int cx, int cy, int radius, SDL_Color &c);
#endif