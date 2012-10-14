
#include "misc.h"
#include <SDL_ttf.h>
using namespace std;


void drawText(string text, int x, int y, TTF_Font *font, SDL_Surface* dest, int r, int g, int b ,TextAlign a){
	
	//gets width & height
	int width, height;
	TTF_SizeText(font,text.c_str(),&width,&height);
	
	switch(a){
	case Centered:{
		x -= width/2;
		y -= height/2;

				  }break;
	case TopLeft:{

				 }break;
	}

	SDL_Color c = {r,g,b};
	SDL_Rect Drect = {x,y,width,height};
	SDL_Surface *memoryLeak = TTF_RenderText_Solid(font,text.c_str(),c);
	SDL_BlitSurface(memoryLeak,NULL,dest,&Drect);
	SDL_FreeSurface(memoryLeak);
}
void drawText(string text, int x, int y, string family, int size, SDL_Surface* dest, int r, int g, int b, TextAlign a ){
	drawText(text,x,y,TTF_OpenFont(family.c_str(),size),dest,r,g,b,a);	
}
void drawText(string text, int x, int y, char *family, int size, SDL_Surface* dest, int r, int g, int b, TextAlign a ){
	drawText(text,x,y,TTF_OpenFont(family,size),dest,r,g,b,a);
}

extern void drawPixel(SDL_Surface *s, int x, int y, int r, int g, int b){
	Uint32 *pixmem32;
    Uint32 colour;  
 
    colour = SDL_MapRGB( s->format, r, g, b );
  
    pixmem32 = (Uint32*) s->pixels  + y*s->w + x;
    *pixmem32 = colour;
}

extern void drawPixel(SDL_Surface *s, int x, int y, SDL_Color &c){
	Uint32 *pixmem32;
    Uint32 colour;  
 
    colour = SDL_MapRGB( s->format, c.r, c.g, c.b );
  
    pixmem32 = (Uint32*) s->pixels  + y*s->w + x;
    *pixmem32 = colour;
}

string intToString(int number)
{
    if (number == 0)
        return "0";
    string temp="";
    string returnvalue="";
    while (number>0)
    {
        temp+=number%10+48;
        number/=10;
    }
    for (int i=0;i<temp.length();i++)
        returnvalue+=temp[temp.length()-i-1];
    return returnvalue;
}

void DrawCircle8(SDL_Surface *s, int x, int y, int cx, int cy, int r, int g, int b, int a ) {
drawPixel(s, x+cx, y+cy, r, g, b); drawPixel(s, -x+cx, y+cy, r, g, b);
drawPixel(s, x+cx, -y+cy, r, g, b); drawPixel(s, -x+cx, -y+cy, r, g, b);

drawPixel(s, y+cy, x+cx, r, g, b);drawPixel(s, y+cy, -x+cx, r, g, b);
drawPixel(s, -y+cy, x+cx, r, g, b);drawPixel(s, -y+cy, -x+cx, r, g, b);

}

void DrawCircle(SDL_Surface *s, int posx, int posy, int radius, int r, int g, int b, int a) {
int rs2 = radius*radius*4; /* this could be folded into ycs2 */
int xs2 = 0;
int ys2m1 = rs2 - 2*radius + 1;
int x = 0;
int y = radius;
int ycs2;
DrawCircle8(s, x, y , posx, posy,r,g,b,a);
while( x <= y ) {
    /* advance to the right */
    xs2 = xs2 + 8*x + 4;
    ++x;
    /* calculate new Yc */
    ycs2 = rs2 - xs2;
    if( ycs2 < ys2m1 ) {
    ys2m1 = ys2m1 - 8*y + 4;
    --y;
    }
    DrawCircle8(s, x, y , posx, posy,r,g,b,a);
}
}
void DrawCircle(SDL_Surface *s, int posx, int posy, int radius, SDL_Color &c) {
	DrawCircle(s,posx,posy,radius,c.r,c.g,c.b,c.unused);
}
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

void fillCircle(SDL_Surface *s, int cx, int cy, int radius, int r, int g, int b, int a ){
	int r2 = radius*radius;
	for(int x = 0; x < radius; x++){
		int yBound = sqrt(r2-x*x)+0.5;
		for(int y = -yBound+1; y < yBound; y++){
			drawPixel(s,x+cx,y+cy,r,g,b);
			drawPixel(s,-x+cx,y+cy,r,g,b);
		}
	}
}
void fillCircle(SDL_Surface *s, int cx, int cy, int radius, SDL_Color &c){
	fillCircle(s,cx,cy,radius,c.r,c.g,c.b,c.unused);
}
bool within(int x, int y, int w, int h, int x1, int y1){
	if(((x1>=x)+(x1>x+w))==1&&((y1>=y)+(y1>y+h))==1) return true;
	return false;
}